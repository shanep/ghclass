#include <vector>
#include <filesystem>
#include <gtest/gtest.h>
#include <ghclass.hpp>

TEST(LoadRepos, individual)
{
     std::vector<Repo> repos = parse_file("./classroom_roster.csv", "shanep-classroom", "project1");
     EXPECT_EQ(repos.size(), 2);
     EXPECT_TRUE(repos[0].repo_id() == "BSU-ShanePanter");
     EXPECT_TRUE(repos[1].repo_id() == "student1");
     EXPECT_TRUE(repos[0].repo_name() == "git@github.com:shanep-classroom/project1-BSU-ShanePanter");
     EXPECT_TRUE(repos[1].repo_name() == "git@github.com:shanep-classroom/project1-student1");
}

TEST(LoadRepos, group)
{
     std::vector<Repo> repos = parse_file("./classroom_roster_groups.csv", "shanep-classroom", "project1");
     EXPECT_EQ(repos.size(), 4);
     EXPECT_TRUE(repos[0].repo_id() == "team-001");
     EXPECT_TRUE(repos[1].repo_id() == "team-001");
     EXPECT_TRUE(repos[2].repo_id() == "team-002");
     EXPECT_TRUE(repos[3].repo_id() == "team-002");
     EXPECT_TRUE(repos[0].repo_name() == "git@github.com:shanep-classroom/project1-team-001");
     EXPECT_TRUE(repos[1].repo_name() == "git@github.com:shanep-classroom/project1-team-001");
     EXPECT_TRUE(repos[2].repo_name() == "git@github.com:shanep-classroom/project1-team-002");
     EXPECT_TRUE(repos[2].repo_name() == "git@github.com:shanep-classroom/project1-team-002");
}

TEST(LiveDownload, individual)
{
     // Three students are in the live test but only 1 repo is valid.
     std::vector<Repo> repos = parse_file("./classroom_roster_live.csv", "shanep-githubclassroom-testing", "lab01");
     EXPECT_EQ(repos.size(), 3);
     EXPECT_TRUE(repos[0].repo_name().empty());
     EXPECT_TRUE(repos[1].repo_name() == "git@github.com:shanep-githubclassroom-testing/lab01-BSU-ShanePanter");
     EXPECT_TRUE(repos[2].repo_name().empty());

     // repos 0 and 2 should fail because they have not been "accepted"
     EXPECT_FALSE(repos[0].clone_repo());
     EXPECT_FALSE(repos[2].clone_repo());

     // remove any existing downloads from previous test runs
     const std::string dest = repos[1].repo_dest_path();
     auto fp = std::filesystem::path(repos[1].repo_dest_path());
     std::filesystem::remove_all(fp);

     EXPECT_TRUE(repos[1].clone_repo());
     EXPECT_TRUE(std::filesystem::exists(fp));
}

TEST(LiveDownload, group)
{
     // Three students are in the live test 2 are on the same team
     std::vector<Repo> repos = parse_file("./classroom_roster_groups_live.csv", "shanep-githubclassroom-testing", "lab02-group");
     EXPECT_EQ(repos.size(), 3);
     EXPECT_TRUE(repos[0].repo_name().empty());
     EXPECT_TRUE(repos[1].repo_name() == "git@github.com:shanep-githubclassroom-testing/lab02-group-team-1");
     EXPECT_TRUE(repos[2].repo_name() == "git@github.com:shanep-githubclassroom-testing/lab02-group-team-1");

     EXPECT_FALSE(repos[0].clone_repo());

     // remove any existing downloads from previous test runs
     const std::string dest = repos[1].repo_dest_path();
     auto fp = std::filesystem::path(repos[1].repo_dest_path());
     std::filesystem::remove_all(fp);
     EXPECT_TRUE(repos[1].clone_repo());
     EXPECT_TRUE(std::filesystem::exists(fp));

     // now when we attempt the next student who is on the same team the clone shut succeed
     //because it is already there
     EXPECT_TRUE(repos[2].clone_repo());
     EXPECT_TRUE(std::filesystem::exists(fp));
}
