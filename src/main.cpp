#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <vector>
#include "ghclass.hpp"

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

     //Get the list of repos and check them out.
     std::vector<Repo> repos = parse_file(roster, org, assignment);
     for (const Repo &r : repos)
     {
          r.clone_repo();
     }
     return 0;
}