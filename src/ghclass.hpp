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
     /* 
     Properies below are mapped directly to the classroom_roster.csv file
     EXAMPLE CSV file:
     "identifier","github_username","github_id","name","group_name"
     "shanepanter@u.boisestate.edu","BSU-ShanePanter","123456","shane","team-001"
     */

     /**
      * @brief The students identifier. This should be their student email. 
      */
     std::string identifier;
     /**
      * @brief The students github user name
      */
     std::string github_username;
     /**
      * @brief The students github id.
      * 
      */
     std::string github_id;
     /**
      * @brief The students name. This may be blank if the student has not set it up
      * 
      */
     std::string name;
     /**
      * @brief If this is a group repo this will have the name of the group.
      * 
      */
     std::string group_name;
     /**
      * @brief Organization is the github organization that you are hosting the classroom
      */
     std::string organization;
     /**
      * @brief The name of the assignment as it appears in github classroom
      */
     std::string assignment;
};

/**
 * @brief Parses a github classroom file and creates a Repo class for each entry
 * 
 * @param fle The github classroom roster (e.g. classroom_roster.csv)
 * @param org The github organization (e.g. shanep-classroom)
 * @param assignment The assignment we want to download (e.g. project1)
 * @return std::vector<Repo> A vector of repos that can be cloned
 */
std::vector<Repo> parse_file(std::string fle, std::string org, std::string assignment);
