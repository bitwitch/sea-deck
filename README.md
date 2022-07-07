### C Declaration Parser
This program parses a C declaration and translates it to an English sentence.  
It is an programming challenge issued in the book *Expert C Programming:  
Deep C Secrets* by Peter Van Der Linden   
 
#### Build
`$ gcc -o cdecl sea_deck.c`

#### Example
Input:  char *(*operators[256])(void);  
Output: operators is a array of 256 pointer to function returning pointer to char  

#### Missing Features
- Parsing arguments to functions  
- Handling struct, enum, and union declarations  
