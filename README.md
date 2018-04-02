# C Assignment

# Task 1 (Hash lookup generator)
* This program takes in 3 parameters: wordlist filename, a integer representing the minimum length of the password and another integer representing the maximum length of the password.
* This program uses the crypt.h library and its functions to create a simple MD5 and SHA-512 hash from a password in the wordlist file (specified on the command line).
* Afterwards, the program writes the two respective hashes of the password into mytab2411.txt.
  # Demo (Task 1)
  ![Compiling and Executing task1](https://github.com/Javiery3889/C-Hash-Lookup-Utility/blob/master/images/task1a_3151_6682.jpg)
  > Execution time: 3-5mins depending on computing power (using wordlist.txt)
  
![Snippet of mytab2411.txt](https://github.com/Javiery3889/C-Hash-Lookup-Utility/blob/master/images/task1b_3151_6682.jpg)
* Possible improvements/features
  * Add multithreading to enable reduce wait time when executing the program.
  * Error handling of wordlist file (e.g check to see if the file is a .txt file)

