#include <stdio.h>
#include <string.h>

void check_access(char *user_input) {
    // Locker 2: The Control Data (0 = Denied, Any other number = Granted)
    int is_admin = 0; 
    
    // Locker 1: The Safe Zone (Only room for 8 characters)
    char buffer[8];   

    printf("Original is_admin value: %d\n", is_admin);

    // --- THE VULNERABILITY ---
    // strcpy is a "blind robot". It copies the user_input into the buffer
    // letter by letter until it sees a null terminator ('\0'). 
    // It NEVER checks if the buffer is actually big enough to hold it!
    strcpy(buffer, user_input);

    printf("Password entered: %s\n", buffer);

    // Normally, the user has to guess this exact password to change is_admin
    if (strcmp(buffer, "MySecret") == 0) {
        is_admin = 1;
    }

    // --- THE HACKER'S CHECK ---
    // If the overflow changed the is_admin variable, this will trigger!
    if (is_admin != 0) {
        printf("\n[!] ACCESS GRANTED. Welcome Admin.\n");
        // Print out the corrupted value of is_admin to prove the overflow
        printf("[!] The is_admin flag was overwritten with: 0x%08x\n", is_admin); 
    } else {
        printf("\n[-] Access Denied.\n");
    }
}

int main(int argc, char *argv[]) {
    // Make sure the user provided a password argument
    if (argc < 2) {
        printf("Usage: %s <password>\n", argv[0]);
        return 1;
    }

    check_access(argv[1]);
    return 0;
}
