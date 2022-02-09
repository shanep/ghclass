#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <filesystem>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "ghclass.hpp"

Repo::Repo(std::string line, std::string org, std::string assign)
    : organization(org), assignment(assign)
{
     std::istringstream iss(line);
     std::string token;
     int count = 0;
     while (std::getline(iss, token, ','))
     {
          // remove any ". This assumes that quotes are not used in team names or github user names
          auto noquotes = std::remove(token.begin(), token.end(), '"');
          token.erase(noquotes, token.end());

          switch (count)
          {
          case 0:
               this->identifier = token;
               break;
          case 1:
               this->github_username = token;
               break;
          case 2:
               this->github_id = token;
               break;
          case 3:
               this->name = token;
               break;
          case 4:
               // Groups can have spaces in them and need to be replaces with -
               std::replace(token.begin(), token.end(), ' ', '-');
               this->group_name = token;
               break;
          }
          count++;
     }
}

std::string Repo::repo_id() const
{
     if (!this->group_name.empty())
     {
          return this->group_name;
     }

     // Default to individual repos
     return this->github_username;
}

std::string Repo::student_id() const
{
     if(this->identifier.empty()){
          return "Unknown - check your roster";
     }
     return this->identifier;
}

std::string Repo::repo_name() const
{
     if (this->github_username.empty())
     {
          return std::string("");
     }
     // build the URL
     std::string rval("git@github.com:");
     rval += this->organization;
     rval += "/";
     rval += this->assignment;
     rval += "-";
     rval += repo_id();
     return rval;
}

std::string Repo::repo_dest_path() const
{
     char *cwd = getcwd(NULL, 0);
     std::string dest(cwd);
     free(cwd);
     dest += "/";
     if (!this->group_name.empty())
     {
          dest += this->group_name;
     }
     else
     {
          dest += this->identifier;
     }
     return dest;
}

/**
 * Do the actual clone operation
 */
bool Repo::clone_repo() const
{
     if (this->repo_name().empty())
     {
          //Student has not accepted the assignment so we can't clone anything.
          return false;
     }
     // check if destination already exists because it was already cloned (group)
     const std::string dest = repo_dest_path();
     if(std::filesystem::exists(std::filesystem::path(dest)))
     {
          std::cerr << "Repo is already cloned!" << std::endl;
          return true;
     }

     pid_t child = fork();
     if (child == -1)
     {
          std::cerr << "fork error! something is horribly wrong with your computer!" << std::endl;
          abort();
     }

     if (child == 0)
     {
          // exec git to clone the repo
          const std::string to_clone = repo_name();        
          std::cout << "cloning: " << to_clone << std::endl;
          execlp("git", "git", "clone", to_clone.c_str(), dest.c_str(), (char *)NULL);
          std::cerr << "Failed to exec!" << std::endl;
          std::cerr << "-->repo name was: " << to_clone << std::endl;
          std::cerr << "-->destination was: " << dest << std::endl;
          exit(1);
     }

     pid_t w;
     int wstatus;
     // Wait for the clone operation to finish. Maybe convert this to async??
     do
     {
          w = waitpid(child, &wstatus, WUNTRACED | WCONTINUED);
          if (w == -1)
          {
               std::cerr << "waitpid returned -1 the clone operation may have failed" << std::endl;
          }
          if (WIFEXITED(wstatus))
          {
               printf("exited, status=%d\n", WEXITSTATUS(wstatus));
          }
          else if (WIFSIGNALED(wstatus))
          {
               printf("killed by signal %d\n", WTERMSIG(wstatus));
          }
          else if (WIFSTOPPED(wstatus))
          {
               printf("stopped by signal %d\n", WSTOPSIG(wstatus));
          }
          else if (WIFCONTINUED(wstatus))
          {
               printf("continued\n");
          }
     } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

     return WIFEXITED(wstatus);
}

std::vector<Repo> parse_file(std::string fle, std::string org, std::string assignment)
{
     std::ifstream roster(fle);
     std::vector<Repo> repos;
     if (roster.is_open())
     {
          std::string line;
          // rip off the first line as it is just the header
          std::getline(roster, line);
          while (std::getline(roster, line))
          {
               // ignore any blank lines
               // Each line should be in the form shown below
               //"shanepanter@u.boisestate.edu","BSU-ShanePanter","123456","shane","team-001"
               if (line.find_first_not_of(' ') != std::string::npos)
               {
                    Repo r(line, org, assignment);
                    repos.push_back(r);
               }
          }
          roster.close();
     }
     else
     {
          std::cerr << "Could not open roster: " << fle << std::endl;
     }
     return repos;
}
