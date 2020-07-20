#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include <git2.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int credentials_cb(git_cred **out, const char *url, const char *username_from_url,
                   unsigned int allowed_types, void *payload)
{
    #define KEYDIR 256
    //grab the home dir for the ssh keys.
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    char privatekey[KEYDIR];
    char publickey[KEYDIR];
    snprintf(privatekey,KEYDIR,"%s/%s",homedir,".ssh/id_rsa");  
    snprintf(publickey,KEYDIR,"%s/%s",homedir,".ssh/id_rsa.pub");

   return git_cred_ssh_key_new(out,username_from_url,publickey,privatekey,"");
}

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
            return std::string("");
        }
        //build the URL
        std::string rval("git@github.com:");
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
        if (!r.empty())
        {
            char *cwd = getcwd(NULL,PATH_MAX);
            std::string dest(cwd);
            free(cwd);
            dest += "/";
            dest += this->identifier;
            std::cout << "Cloning: " << r << " to: " << dest << std::endl;
            git_repository *repo = nullptr;
            git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
            clone_opts.fetch_opts.callbacks.credentials = credentials_cb;
            int error = git_clone(&repo, r.c_str(), dest.c_str(), &clone_opts);
            if (error < 0)
            {
                //TODO: Update this call when libgit2 is updated on onyx
                const git_error *e = giterr_last();
                printf("Error %d/%d: %s\n", error, e->klass, e->message);
            }
            if (repo)
                git_repository_free(repo);
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

    //Get the list of repos
    std::vector<Repo> repos = parse_file(roster, org, assignment);
    git_libgit2_init();
    //check them all out
    for (const Repo &r : repos)
    {
        r.checkout();
    }
    git_libgit2_shutdown();
    return 0;
}
