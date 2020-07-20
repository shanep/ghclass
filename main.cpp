#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <git2.h>

struct Repo
{
    Repo(std::string line, std::string org, std::string assign) : organization(org), assignment(assign)
    {
        std::istringstream iss(line);
        std::string token;
        int count = 0;
        while (std::getline(iss, token, ','))
        {
            //remove the leading and trailing "
            token.pop_back();
            token.replace(0, 1, "");

            switch (count)
            {
            case 0: //identifier
                this->identifier = token;
                break;
            case 1: //github_username
                this->github_username = token;
                break;
            case 2: //github_id
                this->github_id = token;
                break;
            case 3: //name
                this->name = token;
                break;
            case 4: //group
                //Groups can have spaces in them and need to be replaces with -
                std::replace(token.begin(), token.end(), ' ', '-');
                this->group_name = token;
                break;
            }
            count++;
        }
    }

    std::string repo_name() const
    {
        if (this->github_username.empty())
        {
            std::cout << "Student: " << this->identifier << " has not accepted the assignment!"
                      << std::endl;
            return std::string("");
        }
        //build the URL
        std::string rval("github.com/");
        rval += this->organization;
        rval += "/";
        rval += this->assignment;
        rval += "-";
        if (!this->group_name.empty())
        {
            rval += this->group_name;
        }
        else if (!this->github_username.empty())
        {
            rval += this->github_username;
        }
        return rval;
    }

    void checkout() const
    {
        const std::string r = repo_name();
        if(!r.empty()){
            
            std::cout << r <<std::endl;
        }

    }

    std::string identifier;
    std::string github_username;
    std::string github_id;
    std::string name;
    std::string group_name;
    std::string organization;
    std::string assignment;
};

static void usage()
{
    std::cout << std::left << std::setw(30) << "-r <class roster>"
              << ":[required] File name of the class roster (ex. class_roster.csv)"
              << std::endl;
    std::cout << std::left << std::setw(30) << "-o <organization>"
              << ":[required] Name of your Github organization"
              << std::endl;
    std::cout << std::left << std::setw(30) << "-a <assignment name>"
              << ":[required] Name of the assignment"
              << std::endl;
    std::cout << std::left << std::setw(30) << "-h"
              << ":Help"
              << std::endl;
    exit(1);
}

static std::vector<Repo> parse_file(std::string fle, std::string org, std::string assignment)
{
    std::ifstream roster(fle);
    std::vector<Repo> repos;
    if (roster.is_open())
    {
        std::string line;
        //rip off the first line as it is just the header
        std::getline(roster, line);
        while (std::getline(roster, line))
        {
            Repo r(line, org, assignment);
            repos.push_back(r);
        }
        roster.close();
    }
    else
    {
        std::cerr << "Could not open roster: " << fle << std::endl;
    }
    //C++11 should return using move semantics :)
    return repos;
}

int main(int argc, char *argv[])
{
    int opt, s;
    if (argc == 1)
    {
        usage();
        return 0;
    }
    std::string org, assignment, roster;
    while ((opt = getopt(argc, argv, "r:o:a:h")) != EOF)
    {
        switch (opt)
        {
        case 'r':
            roster = optarg;
            break;
        case 'o':
            org = optarg;
            break;
        case 'a':
            assignment = optarg;
            break;
        case '?':
        case 'h':
            usage();
            break;
        }
    }
    if (roster.empty() || org.empty() || assignment.empty())
    {
        std::cerr << "Required argument missing " << std::endl;
        usage();
    }
    git_libgit2_init();

    //Get the list of repos
    std::vector<Repo> repos = parse_file(roster, org, assignment);
    //check them all out
    for (const Repo &r : repos)
    {
        r.checkout();
    }
    git_libgit2_shutdown();
    return 0;
}
