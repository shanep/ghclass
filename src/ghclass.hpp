#include <string>
/**
 * Represents a Git repo in classroom.github.com
 */
class Repo
{
public:
     /**
      * @brief Construct a new Repo:: Repo object
      *
      * @param line A string representation of the repo
      * @param org The github organization that this repo belongs to
      * @param assign The github classroom assignment name
      */
     Repo(std::string line, std::string org, std::string assign);

     /**
      * @brief If the group name is set then we are checking out a group otherwise we are checking
      * out an individual. This function will return the correct ID if the repo is a group repo or
      * note.
      *
      * @return The id of the repo
      */
     std::string repo_id() const;

     /**
      * @brief Returns the students identifier. Typically the students email address
      *
      * @return the identifier
      */
     std::string student_id() const;

     /**
      * @brief Construct the repo name url based on the naming conventions specifed by
      * classroom.github.com returns an empty string if the student has not accepted the
      * assignment.
      *
      * @return The url of the repo
      */
     std::string repo_name() const;

     /**
      * @brief Clone this repo
      *
      * @return true If the clone succeeded
      * @return false If the clone failed
      */
     bool clone_repo() const;

     /**
      * @brief Returns the path to the repo destination. For individual repos this will be
      * the students email and for groups this will be the team name.
      *
      * @return The fully qualified path to the repo.
      */
     std::string repo_dest_path() const;

private:
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
 * EXAMPLE CSV file with one entry:
 *   "identifier","github_username","github_id","name","group_name"
 *   "shanepanter@u.boisestate.edu","BSU-ShanePanter","123456","shane","team-001"
 *
 * @param fle The github classroom roster (e.g. classroom_roster.csv)
 * @param org The github organization (e.g. shanep-classroom)
 * @param assignment The assignment we want to download (e.g. project1)
 * @return std::vector<Repo> A vector of repos that can be cloned
 */
std::vector<Repo> parse_file(std::string fle, std::string org, std::string assignment);
