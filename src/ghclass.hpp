#include <string>
/**
 * Represents a Git repo in classroom.github.com
 */
class Repo
{
public:
     Repo(std::string line, std::string org, std::string assign);

     /**
      * If the group name is set then we are checking out a group
      * otherwise we are checking out an individual
      */
     std::string repo_id() const;

     /**
      * Construct the repo name url based on the
      * naming conventions specifed by classroom.github.com
      */
     std::string repo_name() const;

     /**
      * Do the actual clone operation
      */
     void clone_repo() const;
     
private:
     std::string identifier;
     std::string github_username;
     std::string github_id;
     std::string name;
     std::string group_name;
     std::string organization;
     std::string assignment;
};

std::vector<Repo> parse_file(std::string fle, std::string org, std::string assignment);
