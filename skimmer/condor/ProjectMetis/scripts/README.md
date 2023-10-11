### Useful executables
The scripts here can be run with the `-h` option to get further documentation. Only general descriptions 
are provided below.

#### _manalyze_
Analyze a condor log file for a Metis job.

#### _mclean_
Multiple scripts using metis to submit jobs will write to the same
summary JSON files, appending new tasks to prevent any kind of 
clobbering. This can become a pain when looking at the dashboard
(or using msummary) which will show old/irrelevant tasks. 

This script lets you clean up the jsons. Ex.,
`mclean MINIAODSIM`
will tell you what datasets in the jsons match your pattern MINIAODSIM
and tell you to re-run the command with `--rm` tacked on if you want
to get rid of those.

If you like to see the njob vs time plot on the dashboard, you'll eventually
want to trim the timestamps so you're not staring at a plot going back months
with no ability to see trends in the past few days. 

You can do
`mclean nomatch -d 15`
which will prompt you to delete *no* tasks (unless something matchines nomatch),
but that's ok. We want to keep everything, but only retain the last 15
days of timestamps (`-d 15`) for current tasks. Then similarly add `--rm`
if you're sure you want to modify the jsons.

#### _mdoc_
This is a super lightweight documentation creator for Metis! Run it
to get an HTML file with some useful documentation of Metis objects.


#### _minit_
Run it for fun. Does nothing super useful, other than fun. Fun can be useful.


#### _msummary_
Provides a summary of ongoing Metis tasks.


#### _mtarfile_
Used to make a tarfile for Metis submission.

#### _mtest_
Runs all unit tests.

