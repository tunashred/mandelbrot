# Problem statement

While recursively traversing through the source directory, copy it's contents to the destination directory. For each entry in the source directory, the following operations will be performed based on the type of entry:

**1. Directories:**

An equivalent directory will be created in the destination directory, inheriting the same permissions as the original directory.

**2. Regular files:**

Depending on the size of the files:
	- For files smaller than 500 bytes, a copy of the file will be created in the corresponding place of the destination directory. These copied files will retain only the read permissions. **('-r--r--r-')**

	- For files bigger than 500 bytes, symbolic links will be created poiting to the original files.

**3. Symbolic links:**

They will be copied to the destination directory.
