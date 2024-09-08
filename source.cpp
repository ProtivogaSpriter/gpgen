#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <random>
#include <io.h>
#include <fcntl.h>  //FUCK THAT'S A LOT OF LIBRARIES
#include <locale>

#if defined (_WIN32)
    #include <Windows.h>
    #include <conio.h>
    #include <cstdio>
    #include <codecvt>

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
wchar_t getch(int echo)
{
  wchar_t ch;
  initTermios(echo);
  ch = getwchar();
  resetTermios();
  return ch;
}


#endif

    constexpr unsigned long long poolmaxdisplay {100};

    std::random_device  randomizer_seeder;
    std::mt19937 randomizer(0);

    wchar_t in_char                         {' '};

    std::wstring        input               {L""};

    std::wstring        chr_pool            {L""};
    bool                chr_pool_ovflw      {false};

    std::wstring        pswd_result         {L""};

    const std::wstring  chr_lowercase       {L"abcdefghijklmnopqrstuvwxyz"};
    bool                lowercased          {false};
    const std::wstring  chr_uppercase       {L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    bool                uppercased          {false};
    const std::wstring  chr_numeric         {L"1234567890"};
    bool                numericd            {false};
    const std::wstring  chr_special         {LR"(`~!@#$%^&*()-_=+[{]};:'"\|,<.>/?)"};
    bool                speciald            {false};

    int                 pswd_length         {8};
    unsigned long long  pswd_seed           {randomizer_seeder()};

void clr_csl()
{
#if defined(_WIN32)

    system("cls");

#elif defined (UNIX) ||(_unix_)||(_linux_)||(LINUX)||(linux)

    std::wcout << L"\033[H\033[J";

#endif
}

void keystroke_read(wchar_t& in_char){

#if defined(_WIN32)

    in_char = _getch();
    //std::wcout << "keystroke: " << in_char << std::endl;


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

        std::wcout <<L"╔═══════════════════════╦";

        for(long unsigned int i = 0; i < render_length; i++){
            std::wcout << L"═";
        }
        std::wcout << L"╗\n";

        std::wcout << L"║Current character pool:║";

        if(!chr_pool_ovflw){
            for(long unsigned int i = 0; i < render_length; i++){
                std::wcout << chr_pool[i];
            }
        }
        else{
            for(long unsigned int i = 0; i < poolmaxdisplay; i++){
                std::wcout << chr_pool[i];
            }
            std::wcout << L"\033[96m...\033[0m";
        }
        std::wcout << L"║\n";

        std::wcout << L"╚═══════════════════════╩";

        for(long unsigned int i = 0; i < render_length; i++){
            std::wcout << L"═";
        }
        std::wcout << L"╝\n";

    }
    else{
        std::wcout << L"╔═══════════════════════╦═════╗\n║Current character pool:║empty║\n╚═══════════════════════╩═════╝\n";
    }

    if(!lowercased)
        std::wcout << L"Press \033[95m'l'\033[0m to add lowercase characters.  (" << chr_lowercase << ")" << std::endl;

    if(!uppercased)
        std::wcout << L"Press \033[95m'u'\033[0m to add uppercase characters.  (" << chr_uppercase << ")" << std::endl;

    if(!numericd)
        std::wcout << L"Press \033[95m'n'\033[0m to add numeric characters.    (" << chr_numeric << ")" << std::endl;

    if(!speciald)
        std::wcout << L"Press \033[95m's'\033[0m to add special characters.    (" << chr_special << ")" << std::endl;

    std::wcout << L"\nPress \033[95m'!'\033[0m to add your own string of any UTF-8 characters." << std::endl;

    if(chr_pool_ovflw)
        std::wcout << L"\n\033[96mPress \033[95m'F'\033[0m to show the entire character pool.\033[0m" << std::endl;

    std::wstring str_1 {L""};
    std::wstring str_2 {L""};

    str_1 = std::to_wstring(pswd_seed);
    str_2 = std::to_wstring(pswd_length);

    int big_size {0};

    if(str_1.length() > str_2.length()){
        big_size = str_1.length();
    }
    else{
        big_size = str_2.length();
    }

    std::wcout << L"\n╔═══════════════════╦";

    for(int i = 0; i < big_size; i++){
        std::wcout << L"═";
    }
    std::wcout << L"╗\n";

    std::wcout << L"║Current seed:      ║" << pswd_seed;

    for(int i = str_1.length(); i < big_size; i++){
        std::wcout << " ";
    }
    std::wcout << L"║\n";

    std::wcout << L"║Current passlength:║" << pswd_length;

    for(int i = str_2.length(); i < big_size; i++){
        std::wcout << " ";
    }
    std::wcout << L"║\n";

    std::wcout << L"╚═══════════════════╩";

    for(int i = 0; i < big_size; i++){
        std::wcout << L"═";
    }
    std::wcout << L"╝\n";


    std::wcout << L"\nPress \033[95m'L'\033[0m to set passlength." << std::endl;
    std::wcout << L"Press \033[95m'S'\033[0m to manually set seed. \033[91mThis is not recommended.\033[0m" << std::endl;

    std::wcout << L"\nPress \033[95m'enter'\033[0m to generate password with current settings." << std::endl;

    std::wcout << L"\nPress \033[95m'*'\033[0m to reset everything." << std::endl;
    std::wcout << L"Press \033[95m'esc'\033[0m to shutdown." << std::endl;

    std::wcout << L"\n\033[93mPress \033[95m'H'\033[93m or \033[95m'h'\033[93m for help.\033[0m" << std::endl;
}

void print_fullpool(){
    std::wcout << L"╔═══════════════════════════╗\n║The full character pool is:║\n╚═══════════════════════════╝\n" << std::endl;

    std::wcout << chr_pool << std::endl;

    std::wcout << L"\n\033[93mPress any key to return to main menu.\033[0m" << std::endl;

    keystroke_read(in_char);

}

void reset_status(){
    lowercased = false;
    uppercased = false;
    numericd = false;
    speciald = false;
    chr_pool = L"";
    chr_pool_ovflw = false;
    pswd_length = 8;
    pswd_seed = randomizer_seeder();
}

void imbue_file(std::wofstream& file){

#if defined(_WIN32)

    std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);

#elif defined (UNIX) ||(_unix_)||(_linux_)||(LINUX)||(linux)

    std::locale::loc(std::locale(""));

#endif

    file.imbue(loc);

}

void password_generate(){



        if(chr_pool.length() <= 0){
            clr_csl();
            std::wcout << L"Character pool is blank. Add characters to pool before generating password. \n\n\033[93mPress any key to return to main menu.\033[0m" << std::endl;
            keystroke_read(in_char);
            return;
        }

        clr_csl();
        randomizer.seed(pswd_seed);

        for(int i = 0; i < pswd_length; i++){
            pswd_result += chr_pool[randomizer() % chr_pool.length()];
        }
        std::wcout << L"Your password is: \n" << pswd_result << std::endl;

        std::wcout << L"\nSave to 'output.txt'? (Y/n)" << std::endl;

        yeahnah:

        keystroke_read(in_char);

        switch(in_char){

        case('Y'):
        case('y'):
        {

            input = L"";

            std::wofstream output_file("output.txt", std::ios::app);

            imbue_file(output_file);

            if(!output_file.is_open()){
                std::wcout << L"Error! Unable to open 'output.txt'. Password will not be saved." << std::endl;
                exit(1);
                output_file.close();
                break;
            }

            std::wcout << L"Enter a tag or leave blank for no tag: ";
            std::getline(std::wcin, input);
            std::wcout << L"Entered tag: " << input << std::endl;
            if(input.length() > 0){
                output_file << input << L": ";
            }
            input = L"";
            std::wcout << L"we got here!" << std::endl;

            output_file << pswd_result << std::endl;

            std::wcout << L"even here!" << std::endl;

            output_file.close();

            std::wcout << L"somehow even here!" << std::endl;

            break;
        }
        case('N'):
        case('n'):

            break;

        default:

            goto yeahnah;
        }

        pswd_result = L"";

        pswd_seed = randomizer_seeder();

}

void help_me(void){
    clr_csl();

std::wcout <<LR"(
 ░ ░░▒▒▓▓█ GREG'S PASSWORD GENERATOR MANUAL █▓▓▒▒░░ ░ 

Welcome to GPGEN, a.k.a Greg's Password GENerator.

The purpose of this app is creating randomly-generated passwords using pre-defined (or used defined) character sets.


To generate a password:

1.      Add character sets to the character set pool by pressing the )" << L"\033[95m'l'\033[0m, \033[95m'u'\033[0m, \033[95m'n'\033[0m, \033[95m's'\033[0m" << LR"( keys. Note these are case-sensitive.
1.1     You can add your own UTF-8 conforming character pool by pressing )" <<  L"\033[95m'!'.\033[0m " << LR"(
1.2     If the character pool's size exceeds )" << poolmaxdisplay << LR"(, the diplay bar will show a series of )" << L"\033[96mcyan dots\033[0m" << LR"(. To see the entire pool, press )" << L"\033[95m'F'\033[0m" << LR"(. This is case-sensitive.)"<< std::endl;

std::wcout << LR"(1.3     You can set the generated password's length by pressing )" << L"\033[95m'L'\033[0m" << LR"(. This is case-sensitive.
1.4     You can set the seed by pressing )" << L"\033[95m'S'\033[0m" << LR"(. This is case-sensitive and)" << L" \033[91mnot recommended\033[0m." << LR"(
1.5     You can reset all variables by pressing )" << L"\033[95m'*'\033[0m." << LR"(
1.6     You can halt the program by pressing )" << L"\033[95m'esc'\033[0m" << LR"(.

2.      Generate the password by pressing )" << L"\033[95m'enter'\033[0m" << LR"(. Make sure the character set pool has at least 1 character.
2.1     You can save the password to a plaintext file titled 'output.txt' in the directory of the .exe file by pressing )" << L"\033[95m'Y'\033[0m" << LR"( or )" << L"\033[95m'y'\033[0m" << LR"(. Otherwise, press )" << L"\033[95m'N'\033[0m" << LR"( or )" << L"\033[95m'n'\033[0m" << LR"(.
2.1.1   You can add a tag to the saved password by typing anything when prompted for a tag. If no tag is needed, enter nothing.


That's about it.

)" << L"\033[93mPress any key to return to main menu.\033[0m" << std::endl;

    keystroke_read(in_char);

}

int main(){

#if defined (_WIN32)

    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin),  _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
    //SetConsoleOutputCP(CP_UTF8);
    //setvbuf(stdout, nullptr, _IOFBF, 1000);
    std::wcout << L"You are on windows!" << std::endl;

#elif defined (UNIX) ||(_unix_)||(_linux_)||(LINUX)||(linux)

    std::wcout<< L"You are on linux!" << std::endl;

#endif

    clr_csl();

    std::wcout << LR"(
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
                                               
 ░ ░░▒▒▓▓█ GREG'S PASSWORD GENERATOR █▓▓▒▒░░ ░ )"; //god i am so cool

    std::wcout << L"\n\n\033[93mPress any key to continue.\033[0m" << std::endl;

    //exit(1);

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

        std::wcout << L"Enter custom UTF-8 character string:" << std::endl;
        std::getline(std::wcin, input);
        chr_pool += input;
        input = L"";

        break;

    case('F'):
        if(chr_pool_ovflw){
            clr_csl();
            print_fullpool();
        }
        break;

    case('L'):

        clr_csl();
        std::wcout << L"Enter passlength:" << std::endl;
        std::getline(std::wcin, input);
        std::wstringstream(input) >> pswd_length;
        break;

    case('S'):

        clr_csl();
        std::wcout << L"Enter seed:" << std::endl;
        std::getline(std::wcin, input);
        std::wstringstream(input) >> pswd_seed;
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
