#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>

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

/**
 * Do the actual clone operation
 */
bool Repo::clone_repo() const
{
     const std::string r = repo_name();
     if (!r.empty())
     {
          char *cwd = getcwd(NULL, 0);
          std::string dest(cwd);
          free(cwd);
          dest += "/";
          dest += this->identifier;
          std::cout << "Cloning: " << r << " to: " << dest << std::endl;
          // TODO
     }
     return false;
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
