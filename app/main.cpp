#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <vector>
#include <config.h>
#include <ghclass.hpp>

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
               << ":help"
               << std::endl;
     std::cout << std::left << std::setw(30) << "-v"
               << ":version"
               << std::endl;
     exit(1);
}

int main(int argc, char *argv[])
{
     int opt;
     if (argc == 1)
     {
          usage();
          return 0;
     }
     std::string org, assignment, roster;
     while ((opt = getopt(argc, argv, "r:o:a:hv")) != EOF)
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
          case 'v':
               std::cerr << "version: " << ghclass_VERSION_MAJOR << "." << ghclass_VERSION_MAJOR_VERSION_MINOR << std::endl;
               exit(1);
          }
     }
     if (roster.empty() || org.empty() || assignment.empty())
     {
          std::cerr << "Required argument missing " << std::endl;
          usage();
     }

     // Get the list of repos and check them out.
     std::vector<Repo> repos = parse_file(roster, org, assignment);
     for (const Repo &r : repos)
     {
          if (!r.repo_name().empty())
          {
               std::cout << "Attempting to clone: " << r.repo_name() << std::endl;
               if (!r.clone_repo())
               {
                    std::cout << "--> could not clone repo" << std::endl;
               }
          }
          else
          {
               std::cout << "Student: " << r.student_id() << " appears to not have accepted the assignment" << std::endl;
          }
          std::cout << std::endl;
     }
     return 0;
}