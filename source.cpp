#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <random>                   //FUCK THAT'S A LOT OF LIBRARIES

#if defined (_WIN32)
    #include <Windows.h>
    #include <conio.h>
    #include <cstdio>

#elif defined (UNIX) ||(_unix_)||(_linux_)||(LINUX)||(linux)
    #include <unistd.h>
    #include <stdio.h>
    #include <sys/select.h>
    #include <sys/ioctl.h>
    #include <termios.h>


static struct termios old, current;

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

/* Read 1 character - echo defines echo mode */
char getch(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}


#endif

    constexpr unsigned long long poolmaxdisplay {100};

    std::random_device  randomizer_seeder;
    std::mt19937 randomizer(0);

    char in_char                            {' '};

    std::string         input               {""};

    std::string         chr_pool            {""};
    bool                chr_pool_ovflw      {false};

    std::string         pswd_result         {""};

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
#if defined(_WIN32)

    system("cls");

#elif defined (UNIX) ||(_unix_)||(_linux_)||(LINUX)||(linux)

    std::cout << "\033[H\033[J";

#endif
}

void keystroke_read(char& in_char){

#if defined(_WIN32)

    in_char = _getch();
    //std::cout << "keystroke: " << in_char << std::endl;


#elif defined (UNIX) ||(_unix_)||(_linux_)||(LINUX)||(linux)

    in_char = getch(0);

#endif
}

void print_status(){

    if(chr_pool.length() != 0){

        int render_length = ((chr_pool.length() > poolmaxdisplay) ? poolmaxdisplay : chr_pool.length());

        if(chr_pool_ovflw){
            render_length += 3;
        }

        std::cout <<"╔═══════════════════════╦";

        for(long unsigned int i = 0; i < render_length; i++){
            std::cout << "═";
        }
        std::cout << "╗\n";

        std::cout << "║Current character pool:║";

        if(!chr_pool_ovflw){
            for(long unsigned int i = 0; i < render_length; i++){
                std::cout << chr_pool[i];
            }
        }
        else{
            for(long unsigned int i = 0; i < poolmaxdisplay; i++){
                std::cout << chr_pool[i];
            }
            std::cout << "\033[96m...\033[0m";
        }
        std::cout << "║\n";

        std::cout << "╚═══════════════════════╩";

        for(long unsigned int i = 0; i < render_length; i++){
            std::cout << "═";
        }
        std::cout << "╝\n";

    }
    else{
        std::cout << "╔═══════════════════════╦═════╗\n║Current character pool:║empty║\n╚═══════════════════════╩═════╝\n";
    }

    if(!lowercased)
        std::cout << "Press \033[95m'l'\033[0m to add lowercase characters.  (" << chr_lowercase << ")" << std::endl;

    if(!uppercased)
        std::cout << "Press \033[95m'u'\033[0m to add uppercase characters.  (" << chr_uppercase << ")" << std::endl;

    if(!numericd)
        std::cout << "Press \033[95m'n'\033[0m to add numeric characters.    (" << chr_numeric << ")" << std::endl;

    if(!speciald)
        std::cout << "Press \033[95m's'\033[0m to add special characters.    (" << chr_special << ")" << std::endl;

    std::cout << "\nPress \033[95m'!'\033[0m to add your own string of any UTF-8 characters." << std::endl;

    if(chr_pool_ovflw)
        std::cout << "\n\033[96mPress \033[95m'F'\033[0m to show the entire character pool.\033[0m" << std::endl;

    std::string str_1 {""};
    std::string str_2 {""};

    str_1 = std::to_string(pswd_seed);
    str_2 = std::to_string(pswd_length);

    int big_size {0};

    if(str_1.length() > str_2.length()){
        big_size = str_1.length();
    }
    else{
        big_size = str_2.length();
    }

    std::cout << "\n╔═══════════════════╦";

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


    std::cout << "\nPress \033[95m'L'\033[0m to set passlength." << std::endl;
    std::cout << "Press \033[95m'S'\033[0m to manually set seed. \033[91mThis is not recommended.\033[0m" << std::endl;

    std::cout << "\nPress \033[95m'enter'\033[0m to generate password with current settings." << std::endl;

    std::cout << "\nPress \033[95m'*'\033[0m to reset everything." << std::endl;
    std::cout << "Press \033[95m'esc'\033[0m to shutdown." << std::endl;

    std::cout << "\n\033[93mPress \033[95m'H'\033[93m or \033[95m'h'\033[93m for help.\033[0m" << std::endl;
}

void print_fullpool(){
    std::cout << "╔═══════════════════════════╗\n║The full character pool is:║\n╚═══════════════════════════╝\n" << std::endl;

    std::cout << chr_pool << std::endl;

    std::cout << "\n\033[93mPress any key to return to main menu.\033[0m" << std::endl;

    keystroke_read(in_char);

}

void reset_status(){
    lowercased = false;
    uppercased = false;
    numericd = false;
    speciald = false;
    chr_pool = "";
    chr_pool_ovflw = false;
    pswd_length = 8;
    pswd_seed = randomizer_seeder();
}



void password_generate(){



        if(chr_pool.length() <= 0){
            clr_csl();
            std::cout << "Character pool is blank. Add characters to pool before generating password. \n\n\033[93mPress any key to return to main menu.\033[0m" << std::endl;
            keystroke_read(in_char);
            return;
        }

        clr_csl();
        randomizer.seed(pswd_seed);

        for(int i = 0; i < pswd_length; i++){
            pswd_result += chr_pool[randomizer() % chr_pool.length()];
        }
        std::cout << "Your password is: \n" << pswd_result << std::endl;

        std::cout << "\nSave to 'output.txt'? (Y/n)" << std::endl;

        yeahnah:

        keystroke_read(in_char);

        switch(in_char){

        case('Y'):
        case('y'):
        {

            input = "";

            std::ofstream output_file("output.txt", std::ios::app);
            if(!output_file.is_open()){
                std::cout << "Error! Unable to open 'output.txt'. Password will not be saved." << std::endl;
                exit(1);
                output_file.close();
                break;
            }

            std::cout << "Enter a tag or leave blank for no tag: ";
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

}

void help_me(void){
    clr_csl();

std::cout <<R"(
 ░ ░░▒▒▓▓█ GREG'S PASSWORD GENERATOR MANUAL █▓▓▒▒░░ ░ 

Welcome to GPGEN, a.k.a Greg's Password GENerator.

The purpose of this app is creating randomly-generated passwords using pre-defined (or used defined) character sets.


To generate a password:

1.      Add character sets to the character set pool by pressing the )" << "\033[95m'l'\033[0m, \033[95m'u'\033[0m, \033[95m'n'\033[0m, \033[95m's'\033[0m" << R"( keys. Note these are case-sensitive.
1.1     You can add your own UTF-8 conforming character pool by pressing )" <<  "\033[95m'!'.\033[0m " <<R"(
1.2     If the character pool's size exceeds )" << poolmaxdisplay << R"(, the diplay bar will show a series of )" << "\033[96mcyan dots\033[0m" << R"(. To see the entire pool, press )" << "\033[95m'F'\033[0m" << R"(. This is case-sensitive.)"<<std::endl;

std::cout << R"(1.3     You can set the generated password's length by pressing )" << "\033[95m'L'\033[0m" << R"(. This is case-sensitive.
1.4     You can set the seed by pressing )" << "\033[95m'S'\033[0m" << R"(. This is case-sensitive and)" << " \033[91mnot recommended\033[0m." << R"(
1.5     You can reset all variables by pressing )" << "\033[95m'*'\033[0m." << R"(
1.6     You can halt the program by pressing )" << "\033[95m'esc'\033[0m" <<R"(.

2.      Generate the password by pressing )" << "\033[95m'enter'\033[0m" <<R"(. Make sure the character set pool has at least 1 character.
2.1     You can save the password to a plaintext file titled 'output.txt' in the directory of the .exe file by pressing )" << "\033[95m'enter'\033[0m" <<R"( or )" << "\033[95m'y'\033[0m" <<R"(. Otherwise, press )" << "\033[95m'N'\033[0m" <<R"( or )" << "\033[95m'n'\033[0m" <<R"(.
2.1.1   You can add a tag to the saved password by typing anything when prompted for a tag. If no tag is needed, enter nothing.


That's about it.

)" << "\033[93mPress any key to return to main menu.\033[0m" << std::endl;

    keystroke_read(in_char);

}

int main(){

#if defined (_WIN32)

    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
    std::cout << "You are on windows!" << std::endl;

#elif defined (UNIX) ||(_unix_)||(_linux_)||(LINUX)||(linux)

    std::cout<< "You are on linux!" << std::endl;

#endif

    clr_csl();

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

    std::cout << "\n\033[93mPress any key to continue.\033[0m" << std::endl;

    keystroke_read(in_char);

goagain:

    clr_csl();

    if(chr_pool.length() > poolmaxdisplay){
        chr_pool_ovflw = true;
    }

    print_status();

notquite:

    keystroke_read(in_char);

    switch(in_char){

    case('l'):
        if(!lowercased){
            lowercased = true;
            chr_pool += chr_lowercase;
        }
        break;

    case('u'):
        if(!uppercased){
            uppercased = true;
            chr_pool += chr_uppercase;
        }
        break;

    case('n'):
        if(!numericd){
            numericd = true;
            chr_pool += chr_numeric;
        }
        break;

    case('s'):
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

    case('F'):
        if(chr_pool_ovflw){
            clr_csl();
            print_fullpool();
        }
        break;

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
    case('\n'):
        password_generate();
        break;

    case('*'):
        reset_status();
        break;

    case(0x1B):
        clr_csl();
        return(0);

    case('h'):
        help_me();
        break;

    default:
        goto notquite;
    }

    goto goagain;

}
