# C Assignment

# Task 1 (Hash lookup wordlist generator)
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
  
# Task 2 (Password Lookup Program)
* This program takes in 2 parameters, 2 files to be exact: first argument is for the location of the file with the username and the digest of the respective user's password (similar to /etc/shadow file in linux systems). The second file will be the lookup table we will be using containing all the possible password and hash pair. (we will be using mytab2411.txt as the lookup table)
* This program compares each user's password hash with each hash in mytab2411.txt until EOF. The program also checks if the user's password digest is valid by checking the digest length.
  # Demo (Task 2)
  ![contents of shadow.txt](https://github.com/Javiery3889/C-Hash-Lookup-Utility/blob/master/images/task2a_3151_6682.JPG)
  > shadow.txt, which contains user and password digest pairs.
  
  ![Compiling and Executing task2](https://github.com/Javiery3889/C-Hash-Lookup-Utility/blob/master/images/task2b_3151_6682.JPG)
  > Execution time: 3-5mins depending on computing power, pyc3 password could not be found as the hash length does not equal to 90 characters.
  
  * Possible improvements/features
    * hello
    * Add multithreading to enable reduce wait time when executing the program.
  
