from __future__ import print_function

from metis.File import MutableFile

"""
Showcases some file operations normally done using the os
module, but nicely wrapped in the MutableFile object
"""
if __name__ == "__main__":

    # Make a mutable file object
    fo = MutableFile("mutablefile_test.txt")

    # Touch the file to guarantee its existence
    fo.touch()

    # Does it exist? Hint: yes
    print("File exists?", fo.exists())

    # What are the current permissions?
    print("Permissions =", fo.chmod())

    # Add some text to it
    fo.append("test text\n")
    fo.append("more text")

    # And cat it out
    print("---- Begin contents --->")
    print(fo.cat())
    print("<--- End contents ----")


    # Clean up by removing the file
    fo.rm()
