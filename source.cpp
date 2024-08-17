#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <random>                   //FUCK THAT'S A LOT OF LIBRARIES

#if defined _WIN32
    #include <Windows.h>
    #include <conio.h>
    #include <cstdio>

#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    #include <unistd.h>
    #include <stdio.h>
    #include <sys/select.h>
    #include <termios.h>
    #include <stropts.h>
int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}

char _getch(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

#endif

    std::random_device  randomizer_seeder;
    std::mt19937 randomizer(0);

    char in_char {' '};

    std::string input       {""};

    std::string chr_pool    {""};

    std::string pswd_result {""};

    const std::string   chr_lowercase       {"abcdefghijklmnopqrstuvwxyz"};
    bool                lowercased          {false};
    const std::string   chr_uppercase       {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    bool                uppercased          {false};
    const std::string   chr_numeric         {"1234567890"};
    bool                numericd            {false};
    const std::string   chr_special         {R"(`~!@#$%^&*()-_=+[{]};:'"\|,<.>/?)"};
    bool                speciald            {false};

    int                 pswd_length         {8};
    unsigned long long  pswd_seed           {randomizer_seeder()};

void clr_csl()
{
#if defined _WIN32

    system("cls");

#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)

    system("clear");

#endif
}

void keystroke_read(char& in_char){

#if defined _WIN32

    while( !_kbhit()){
        Sleep(3);
    }
    in_char = _getch();
    //std::cout << "keystroke: " << in_char << std::endl;


#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)

    while( !_kbhit()){
        usleep(3000);   //this is in mircoseconds, as opposed to Windows.h's Sleep(), which takes milliseconds
    }
    in_char = _getch(0);
    //std::cout << "keystroke: " << in_char << std::endl;

#endif
}

void print_status(){

    if(chr_pool.length() != 0){

        std::cout <<"╔═══════════════════════╦";

        for(int i = 0; i < chr_pool.length(); i++){
            std::cout << "═";
        }
        std::cout << "╗\n";

        std::cout << "║Current character pool:║";

        for(int i = 0; i < chr_pool.length(); i++){
            std::cout << chr_pool[i];
        }
        std::cout << "║\n";

        std::cout << "╚═══════════════════════╩";

        for(int i = 0; i < chr_pool.length(); i++){
            std::cout << "═";
        }
        std::cout << "╝\n";

    }
    else{
        std::cout << "╔═══════════════════════╦═════╗\n║Current character pool:║empty║\n╚═══════════════════════╩═════╝\n";
    }

    if(!lowercased)
        std::cout << "Press 'L' to add lowercase characters.  (" << chr_lowercase << ")" << std::endl;

    if(!uppercased)
        std::cout << "Press 'U' to add uppercase characters.  (" << chr_uppercase << ")" << std::endl;

    if(!numericd)
        std::cout << "Press 'N' to add numeric characters.    (" << chr_numeric << ")" << std::endl;

    if(!speciald)
        std::cout << "Press 'S' to add special characters.    (" << chr_special << ")" << std::endl;

    std::cout << "\nPress '!' to add your own string of any UTF-8 characters." << std::endl;

    std::cout << "\nPress 'enter' to generate password from current pool." << std::endl;

    std::cout << "\nPress '*' to reset everything." << std::endl;
    std::cout << "Press 'esc' to shutdown." << std::endl;

    std::cout << "\n\033[93mPress 'H' or 'h' for help.\033[0m" << std::endl;
}

void reset_status(){
    lowercased = false;
    uppercased = false;
    numericd = false;
    speciald = false;
    chr_pool = "";
    pswd_length = 8;
    pswd_seed = randomizer_seeder();
}

void password_generate(){

    std::string str_1 {""};
    std::string str_2 {""};

pswd_reset:

    clr_csl();

    str_1 = std::to_string(pswd_seed);
    str_2 = std::to_string(pswd_length);

    int big_size {0};

    if(str_1.length() > str_2.length()){
        big_size = str_1.length();
    }
    else{
        big_size = str_2.length();
    }

    std::cout << "╔═══════════════════╦";

    for(int i = 0; i < big_size; i++){
        std::cout << "═";
    }
    std::cout << "╗\n";

    std::cout << "║Current seed:      ║" << pswd_seed;

    for(int i = str_1.length(); i < big_size; i++){
        std::cout << " ";
    }
    std::cout << "║\n";

    std::cout << "║Current passlength:║" << pswd_length;

    for(int i = str_2.length(); i < big_size; i++){
        std::cout << " ";
    }
    std::cout << "║\n";

    std::cout << "╚═══════════════════╩";

    for(int i = 0; i < big_size; i++){
        std::cout << "═";
    }
    std::cout << "╝\n";


    std::cout << "\nPress 'L' to set passlength. \n\nPress 'S' to manually set seed. \033[91mThis is not recommended.\033[0m \n\nPress 'enter' to generate password with current settings. \n\nPress 'esc' to return to previous menu." << std::endl;

pswd_notquite:

    keystroke_read(in_char);

    switch(in_char){

    case('L'):

        clr_csl();
        std::cout << "Enter passlength:" << std::endl;
        std::getline(std::cin, input);
        std::stringstream(input) >> pswd_length;
        break;

    case('S'):

        clr_csl();
        std::cout << "Enter seed:" << std::endl;
        std::getline(std::cin, input);
        std::stringstream(input) >> pswd_seed;
        break;

    case(0x0D):
    {
        if(chr_pool.length() <= 0){
            clr_csl();
            std::cout << "Character pool is blank. Add characters to pool before generating password. Press any key to return." << std::endl;
            keystroke_read(in_char);
            break;
        }

        clr_csl();
        randomizer.seed(pswd_seed);

        for(int i = 0; i < pswd_length; i++){
            pswd_result += chr_pool[randomizer() % chr_pool.length()];
        }
        std::cout << "Your password is: \n" << pswd_result << std::endl;

        std::cout << "\nSave to 'output.txt'? (Y/N)" << std::endl;

        yeahnah:

        keystroke_read(in_char);

        switch(in_char){

        case('Y'):
        case('y'):
        {


            bool tagexists {false};
            input = "";

            std::ofstream output_file("output.txt", std::ios::app);
            if(!output_file.is_open()){
                std::cout << "Error! Unable to open 'output.txt'. Password will not be saved." << std::endl;
                exit(1);
                output_file.close();
                break;
            }

            std::cout << "Enter a tag or leave blank for no tag: "; //wtf is going on with this piece of shit??? sometimes it works sometimes it doesn't? i'm bout to genuinely shit the bed here
            std::getline(std::cin, input);
            if(input.length() > 0){
                output_file << input << ": ";
            }
            input = "";
            output_file << pswd_result << std::endl;

            output_file.close();



            break;
        }
        case('N'):
        case('n'):

            break;

        default:

            goto yeahnah;
        }

        pswd_result = "";

        pswd_seed = randomizer_seeder();

        break;
    }
    case(0x1B):

        return;

    default:

        goto pswd_notquite;
    }

    goto pswd_reset;

}

void help_me(void){
    clr_csl();

std::cout <<R"(
 ░ ░░▒▒▓▓█ GREG'S PASSWORD GENERATOR MANUAL █▓▓▒▒░░ ░ 

Welcome to GPGEN, a.k.a Greg's Password GENerator.

The purpose of this app is creating randomly-generated passwords using pre-defined (or used defined) character sets.


To generate a password:

1.      Add character sets to the character set pool by using the 'L', 'U', 'N', 'S' keys. Note these are case-sensitive.
1.1     You can add your own UTF-8 conforming character pool by pressing '!'.
1.2     You can reset all variables by pressing '*'.
1.3     You can halt the program by pressing 'esc'.)" << std::endl;

std::cout << R"(
2.      Move to the next menu by pressing 'enter'.
2.1     You can set the generated password's length by pressing 'L'. This is case-sensitive.
2.2     You can set the seed by pressing 'S'. This is case-sensitive and)" << " \033[91mnot recommended\033[0m." << R"(
2.3     You can quit out to the previous menu by pressing 'esc'.

3.      Generate the password by pressing 'enter'. Make sure the character set pool has at least 1 character.
3.1     You can save the password to a plaintext file titled 'output.txt' in the directory of the .exe file by pressing 'Y' or 'y'. Otherwise, press 'N' or 'n'.
3.1.1   You can add a tag to the generated password by typing anything when prompted for a tag. If no tag is needed, enter nothing.


That's about it.


Enter any key to stop reading manual.)" << std::endl;

    keystroke_read(in_char);

}

int main(){

#if defined _WIN32                           //gotta test this on linux too
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);

#endif

    std::cout << R"(
╔═════════════════════════════════════════════╗
║                                             ║
║▒███████ ▒███████ ▒███████ ▒███████ ▒██  ▒██ ║
║▒███████ ▒███████ ▒███████ ▒███████ ▒██  ▒██ ║
║▒██░░░░░ ▒██░░░██ ▒██░░░░░ ▒██░░░░░ ▒██  ▒██ ║
║▒██      ▒██  ▒██ ▒██      ▒██      ▒██  ▒██ ║
║▒██      ▒██  ▒██ ▒██      ▒██      ▒███ ▒██ ║
║▒██ ▒███ ▒███████ ▒██ ▒███ ▒███████ ▒████░██ ║
║▒██ ▒███ ▒███████ ▒██ ▒███ ▒███████ ▒██░████ ║
║▒██  ░██ ▒██░░░░░ ▒██  ░██ ▒██░░░░░ ▒██ ░███ ║
║▒██  ▒██ ▒██      ▒██  ▒██ ▒██      ▒██  ░██ ║
║▒██  ▒██ ▒██      ▒██  ▒██ ▒██      ▒██  ▒██ ║
║▒███████ ▒██      ▒███████ ▒███████ ▒██  ▒██ ║
║▒███████ ▒██      ▒███████ ▒███████ ▒██  ▒██ ║
║░░░░░░░░ ░░░      ░░░░░░░░ ░░░░░░░░ ░░░  ░░░ ║
╚═════════════════════════════════════════════╝
                                               
 ░ ░░▒▒▓▓█ GREG'S PASSWORD GENERATOR █▓▓▒▒░░ ░ )" << std::endl; //god i am so cool

    std::cout << "\nPress any key to continue.\n" << std::endl;

    keystroke_read(in_char);

goagain:

    clr_csl();

    print_status();

notquite:

    keystroke_read(in_char);

    switch(in_char){

    case('L'):
        if(!lowercased){
            lowercased = true;
            chr_pool += chr_lowercase;
        }
        break;

    case('U'):
        if(!uppercased){
            uppercased = true;
            chr_pool += chr_uppercase;
        }
        break;

    case('N'):
        if(!numericd){
            numericd = true;
            chr_pool += chr_numeric;
        }
        break;

    case('S'):
        if(!speciald){
            speciald = true;
            chr_pool += chr_special;
        }
        break;

    case('!'):
        clr_csl();

        std::cout << "Enter custom UTF-8 character string:" << std::endl;
        std::getline(std::cin, input);
        chr_pool += input;
        input = "";

        break;

    case(0x0D):
        password_generate();
        break;

    case('*'):
        reset_status();
        break;

    case(0x1B):
        exit(0);

    case('h'):
        help_me();
        break;

    default:
        goto notquite;
    }

    goto goagain;

}
