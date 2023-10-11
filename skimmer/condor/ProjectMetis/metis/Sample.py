from __future__ import print_function

import logging
import glob
import time
import datetime
import os
import fnmatch
import json

import scripts.dis_client as dis

from metis.Constants import Constants
from metis.Utils import setup_logger, cached, do_cmd
from metis.File import FileDBS, EventsFile, ImmutableFile, MutableFile

DIS_CACHE_SECONDS = 5*60
if os.getenv("NOCACHE"): DIS_CACHE_SECONDS = 0

class Sample(object):
    """
    General sample which stores as much information as we might want
    """

    def __init__(self, **kwargs):
        # Handle whatever kwargs we want here
        self.info = {
            "tier": kwargs.get("tier", "CMS3"),
            "dataset": kwargs.get("dataset", None),
            "gtag": kwargs.get("gtag", None),
            "kfact": kwargs.get("kfact", 1.),
            "xsec": kwargs.get("xsec", 1.),
            "efact": kwargs.get("efact", 1.),
            "filtname": kwargs.get("filtname", None),
            "analysis": kwargs.get("analysis", None),
            "tag": kwargs.get("tag", None),
            "version": kwargs.get("version", None),
            "nevents_in": kwargs.get("nevents_in", None),
            "nevents": kwargs.get("nevents", None),
            "location": kwargs.get("location", None),
            "status": kwargs.get("status", None),
            "twiki": kwargs.get("twiki", None),
            "comments": kwargs.get("comments", None),
            "files": kwargs.get("files", []),
        }

        self.logger = logging.getLogger(setup_logger())

    def __repr__(self):
        return "<{0} dataset={1}>".format(self.__class__.__name__, self.info["dataset"])

    # @cached(default_max_age = datetime.timedelta(seconds=DIS_CACHE_SECONDS))
    def do_dis_query(self, ds, typ="files"):

        self.logger.debug("Doing DIS query of type {0} for {1}".format(typ, ds))

        rawresponse = dis.query(ds, typ=typ, detail=True)
        response = rawresponse["payload"]
        if not len(response):
            self.logger.error("Query failed with response:" + str(rawresponse))

        return response

    def load_from_dis(self):

        (status, val) = self.check_params_for_dis_query()
        if not status:
            self.logger.error("[Dataset] Failed to load info for dataset %s from DIS because parameter %s is missing." % (self.info["dataset"], val))
            return False

        query_str = "status=%s, dataset_name=%s, sample_type=%s" % (Constants.VALID_STR, self.info["dataset"], self.info["type"])
        if self.info["type"] != "CMS3":
            query_str += ", analysis=%s" % (self.info["analysis"])
        if self.info["tag"]:
            query_str += ", cms3tag=%s" % (self.info["tag"])

        response = {}
        try:
            response = dis.query(query_str, typ='snt', detail=True)
            response = response["payload"]
            if len(response) == 0:
                self.logger.error(" Query found no matching samples for: status = %s, dataset = %s, type = %s analysis = %s" % (self.info["status"], self.info["dataset"], self.info["type"], self.info["analysis"]))
                return False

            if len(response) > 1:
                # response = self.sort_query_by_key(response,"timestamp")
                response = self.sort_query_by_key(response,"cms3tag")

            if hasattr(self,"exclude_tag_pattern") and self.exclude_tag_pattern:
                new_response = []
                for samp in response:
                    tag = samp.get("tag", samp.get("cms3tag", ""))
                    if fnmatch.fnmatch(tag,self.exclude_tag_pattern): continue
                    new_response.append(samp)
                response = new_response

            self.info["gtag"] = response[0]["gtag"]
            self.info["kfact"] = response[0]["kfactor"]
            self.info["xsec"] = response[0]["xsec"]
            self.info["filtname"] = response[0].get("filter_name", "NoFilter")
            self.info["efact"] = response[0]["filter_eff"]
            self.info["analysis"] = response[0].get("analysis", "")
            self.info["tag"] = response[0].get("tag", response[0].get("cms3tag"))
            self.info["version"] = response[0].get("version", "v1.0")
            self.info["nevts_in"] = response[0]["nevents_in"]
            self.info["nevts"] = response[0]["nevents_out"]
            self.info["location"] = response[0]["location"]
            self.info["status"] = response[0].get("status", Constants.VALID_STR)
            self.info["twiki"] = response[0].get("twiki_name", "")
            self.info["files"] = response[0].get("files", [])
            self.info["comments"] = response[0].get("comments", "")
            return True
        except:
            return False

    def do_update_dis(self):

        if hasattr(self,"read_only") and self.read_only:
            self.logger.debug("Not updating DIS since this sample has read_only=True")
            return False

        self.logger.debug("Updating DIS")
        query_str = "dataset_name={},sample_type={},cms3tag={},gtag={},location={},nevents_in={},nevents_out={},xsec={},kfactor={},filter_eff={},timestamp={}".format(
           self.info["dataset"], self.info["tier"], self.info["tag"], self.info["gtag"],
           self.info["location"], self.info["nevents_in"], self.info["nevents"],
           self.info["xsec"], self.info["kfact"], self.info["efact"], int(time.time())
        )

        response = {}
        try:
            succeeded = False
            response = dis.query(query_str, typ='update_snt')
            response = response["payload"]
            if "updated" in response and str(response["updated"]).lower() == "true":
                succeeded = True
            self.logger.debug("Updated DIS")
        except:
            pass

        if not succeeded:
            self.logger.debug("WARNING: failed to update sample using DIS with query_str: {}".format(query_str))
            self.logger.debug("WARNING: got response: {}".format(response))

        return succeeded

    def check_params_for_dis_query(self):
        if "dataset" not in self.info:
            return (False, "dataset")
        if "type" not in self.info:
            return (False, "type")
        if self.info["type"] != "CMS3" and "analysis" not in self.info:
            return (False, "analysis")
        return (True, None)

    def sort_query_by_key(self, response, key, descending=True):
        if type(response) is list:
            return sorted(response, key=lambda k: k.get(key, -1), reverse=descending)
        else:
            return response

    def get_datasetname(self):
        return self.info["dataset"]

    def get_nevents(self):
        if self.info.get("nevts", None):
            return self.info["nevts"]
        self.load_from_dis()
        return self.info["nevts"]

    def get_files(self):
        if self.info.get("files", None):
            return self.info["files"]
        self.load_from_dis()
        self.info["files"] = [EventsFile(f) for f in glob.glob(self.info["location"])]
        return self.info["files"]

    def get_globaltag(self):
        if self.info.get("gtag", None):
            return self.info["gtag"]
        self.load_from_dis()
        return self.info["gtag"]



class DBSSample(Sample):
    """
    Sample which queries DBS (through DIS)
    for central samples
    """

    def __init__(self, **kwargs):

        self.allow_invalid_files = kwargs.get("allow_invalid_files", False)
        self.dasgoclient = kwargs.get("dasgoclient", False) # use dasgoclient instead of DIS

        if os.getenv("USEDASGOCLIENT", False):
            self.dasgoclient = True

        super(DBSSample, self).__init__(**kwargs)

    def set_selection_function(self, selection):
        """
        Use this to specify a function returning True for files we
        want to consider only. Input to the selection function is
        the filename
        """
        self.selection = selection

    def load_from_dis(self):

        query = self.info["dataset"]
        if self.allow_invalid_files:
            query += ",all"
        response = self.do_dis_query(query, typ="files")
        fileobjs = [
                FileDBS(name=fdict["name"], nevents=fdict["nevents"], filesizeGB=fdict["sizeGB"]) for fdict in response
                if (not hasattr(self,"selection") or self.selection(fdict["name"]))
                ]
        fileobjs = sorted(fileobjs, key=lambda x: x.get_name())

        self.info["files"] = fileobjs
        self.info["nevts"] = sum(fo.get_nevents() for fo in fileobjs)

    def load_from_dasgoclient(self):

        cmd = "dasgoclient -query 'file dataset={}' -json".format(self.info["dataset"])
        js = json.loads(do_cmd(cmd))
        fileobjs = []
        for j in js:
            f = j["file"][0]
            if (not hasattr(self,"selection") or self.selection(fdict["name"])):
                fileobjs.append(FileDBS(name=f["name"], nevents=f["nevents"], filesizeGB=round(f["size"]*1e-9,2)))
        fileobjs = sorted(fileobjs, key=lambda x: x.get_name())

        self.info["files"] = fileobjs
        self.info["nevts"] = sum(fo.get_nevents() for fo in fileobjs)

    def get_nevents(self):
        if self.info.get("nevts", None):
            return self.info["nevts"]
        if self.dasgoclient:
            self.load_from_dasgoclient()
        else:
            self.load_from_dis()
        return self.info["nevts"]

    def get_files(self):
        if self.info.get("files", None):
            return self.info["files"]
        if self.dasgoclient:
            self.load_from_dasgoclient()
        else:
            self.load_from_dis()
        return self.info["files"]

    def get_globaltag(self):
        if self.info.get("gtag", None):
            return self.info["gtag"]
        if self.dasgoclient:
            cmd = "dasgoclient -query 'config dataset={} system=dbs3' -json".format(self.info["dataset"])
            js = json.loads(do_cmd(cmd))
            response = js[0]["config"][0]
        else:
            response = self.do_dis_query(self.info["dataset"], typ="config")
        self.info["gtag"] = str(response["global_tag"])
        self.info["native_cmssw"] = str(response["release_version"])
        return self.info["gtag"]

    def get_native_cmssw(self):
        if self.info.get("native_cmssw", None):
            return self.info["native_cmssw"]
        response = self.do_dis_query(self.info["dataset"], typ="config")
        self.info["gtag"] = response["global_tag"]
        self.info["native_cmssw"] = response["native_cmssw"]
        return self.info["native_cmssw"]

class DirectorySample(Sample):
    """
    Sample which just does a directory listing to get files
    Requires a `location` to do an ls and a `dataset`
    for naming purposes
    :kwarg globber: pattern to select files in `location`
    :kwarg location: where to pick up files using `globber`
    :kwarg use_xrootd: if `True`, transform filenames into `/store/...`
    """

    def __init__(self, **kwargs):
        # Handle whatever kwargs we want here
        needed_params = self.needed_params()
        if any(x not in kwargs for x in needed_params):
            raise Exception("Need parameters: {0}".format(",".join(needed_params)))

        self.globber = kwargs.get("globber", "*.root")
        self.use_xrootd = kwargs.get("use_xrootd", False)

        # Pass all of the kwargs to the parent class
        super(DirectorySample, self).__init__(**kwargs)

    def needed_params(self):
        return ["dataset","location"]

    def get_files(self):
        if self.info.get("files", None):
            return self.info["files"]
        filepaths = glob.glob(self.info["location"] + "/" + self.globber)
        if self.use_xrootd:
            filepaths = ["/store/"+fp.split("/store/",1)[-1] for fp in filepaths]
        filepaths = sorted(filepaths)
        self.info["files"] = list(map(EventsFile, filepaths))

        return self.info["files"]

    def get_nevents(self):
        return self.info.get("nevts", 0)

    def get_globaltag(self):
        return self.info.get("gtag", "dummy_gtag")

    def set_files(self, fnames):
        if self.use_xrootd:
            fnames = ["/store/"+fp.split("/store/",1)[-1] for fp in fnames]
        self.info["files"] = list(map(EventsFile, fnames))

class SNTSample(DirectorySample):
    """
    Sample object which queries DIS for SNT samples
    :kwarg read_only: if `False`, prevent DIS updating
    :kwarg exclude_tag_pattern: skips samples from DIS with cms3tag matching pattern (must use globs, so `V08` won't work, but `*V08*` will)
    """

    def __init__(self, **kwargs):

        self.typ = kwargs.get("typ", "CMS3")
        self.read_only = kwargs.get("read_only", True)
        self.exclude_tag_pattern = kwargs.get("exclude_tag_pattern", "")
        self.skip_files = kwargs.get("skip_files",None)

        # Pass all of the kwargs to the parent class
        super(SNTSample, self).__init__(**kwargs)

        self.info["type"] = self.typ

    def needed_params(self):
        return ["dataset"]

    def get_nevents(self):
        if self.info.get("nevts", None):
            return self.info["nevts"]
        self.load_from_dis()
        return self.info["nevts"]

    def get_location(self):
        if self.info.get("location", None):
            return self.info["location"]
        self.load_from_dis()
        # If we get here and there's no location, something went wrong...
        if not self.info["location"]:
            raise RuntimeError("Failed to get location for this sample!")
        return self.info["location"]

    def get_files(self):
        if self.info.get("files", None):
            return self.info["files"]
        filepaths = glob.glob(self.get_location() + "/" + self.globber)

        #PRO MOVE : Don't go around skipping files if you're a serial procrastinator!
        if self.skip_files:
            if type(self.skip_files) is not list:
                self.skip_files = [self.skip_files]
            for filename in self.skip_files:
                self.logger.info("Removing {} from list".format(filename))
                filepaths.remove(filename)

        if self.use_xrootd:
            filepaths = [fp.replace("/hadoop/cms", "") for fp in filepaths]

        self.info["files"] = list(map(EventsFile, filepaths))
        fname_metadata = self.get_location() + "/metadata.json"
        if os.path.exists(fname_metadata):
            with open(fname_metadata,"r") as fh:
                metadata = json.load(fh)
                ijob_to_nevents = metadata["ijob_to_nevents"]
            for f in self.info["files"]:
                nevents, nevents_eff = ijob_to_nevents.get(str(f.get_index()),(0,0))
                nevents_neg = (nevents-nevents_eff) // 2
                f.set_nevents(nevents)
                f.set_nevents_negative(nevents_neg)

        return self.info["files"]

    def get_globaltag(self):
        if self.info.get("gtag", None):
            return self.info["gtag"]
        response = self.do_dis_query(self.info["dataset"], typ="config")
        self.info["gtag"] = response["global_tag"]
        self.info["native_cmssw"] = response["release_version"]
        return self.info["gtag"]

class FilelistSample(DirectorySample):
    """
    Sample object made from a filelist (text file, or python list) If elements
    of the "filelist" are pairs, then first slot is assumed to be the filepath
    and the second is the number of events in the file
    """

    def __init__(self, **kwargs):

        self.filelist = kwargs.get("filelist", None)

        # Pass all of the kwargs to the parent class
        super(FilelistSample, self).__init__(**kwargs)

    def needed_params(self):
        return ["dataset","filelist"]

    def get_files(self):
        if self.info.get("files", None):
            return self.info["files"]

        if type(self.filelist) == list:
            filepaths = self.filelist
        else:
            imf = ImmutableFile(self.filelist)
            if not imf.exists(): raise Exception("Filelist {} does not exist!".format(imf.get_name()))
            filepaths = map(lambda x: x.strip(), imf.cat().splitlines())
        filepaths, nevents = self.separate_paths_events(filepaths)

        if self.use_xrootd:
            filepaths = [fp.replace("/hadoop/cms", "") for fp in filepaths]

        if nevents:
            self.info["files"] = list(map(lambda x: EventsFile(x[0], nevents=x[1]), zip(filepaths,nevents)))
        else:
            self.info["files"] = list(map(EventsFile, filepaths))

        return self.info["files"]

    def separate_paths_events(self, thelist):
        if len(thelist) > 0:
            if len(thelist[0]) == 2:
                filepaths, nevents = zip(*thelist)
                nevents = map(int, nevents)
                return filepaths, nevents
        return thelist, []



class DummySample(DirectorySample):
    """
    Dummy sample object made from a number of inputs
    Used for tasks where you have a known number of jobs,
    but no inputs
    """

    def __init__(self, **kwargs):

        self.n_dummy_files = kwargs.get("N", 0)
        self.dummy_name = kwargs.get("name", "dummy")
        self.dummy_extension = kwargs.get("extension", "root")


        # Pass all of the kwargs to the parent class
        super(DummySample, self).__init__(**kwargs)

    def needed_params(self):
        return ["dataset"]

    def get_files(self):
        if self.info.get("files", None):
            return self.info["files"]
        extra = {}
        nevents_per_file = 0
        if self.info.get("nevents",0) > 0:
            nevents_per_file = int(self.info["nevents"] / self.n_dummy_files)
            self.info["nevts"] = self.info["nevents"]
        self.info["files"] = [EventsFile("{}_{}.{}".format(self.dummy_name,i,self.dummy_extension),fake=True,nevents=nevents_per_file) for i in range(self.n_dummy_files)]
        return self.info["files"]


if __name__ == '__main__':

    s1 = SNTSample(dataset="/MET/Run2016B-17Jul2018_ver1-v1/MINIAOD")
    print(s1.get_files())
    print(len(s1.get_files()))

    # s1 = DBSSample(dataset="/DoubleMuon/Run2017A-PromptReco-v3/MINIAOD")
    # s1.set_selection_function(lambda x: "296/980/" in x)
    # print(len(s1.get_files()))
    # print(s1.get_nevents())

    # s1 = DBSSample(dataset="/MET/Run2017A-PromptReco-v3/MINIAOD")
    # print(len(s1.get_files()))

    # s1 = DBSSample(dataset="/JetHT/Run2017A-PromptReco-v3/MINIAOD")
    # print(len(s1.get_globaltag()))

    # ds = DirectorySample(
    #         dataset="/blah/blah/MINE",
    #         location="/hadoop/cms/store/user/namin/ProjectMetis/JetHT_Run2017A-PromptReco-v3_MINIAOD_CMS4_V00-00-03",
    #         )
    # print ds.get_files()
    # print ds.get_globaltag()
    # print ds.get_datasetname()


