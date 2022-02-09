#include <vector>
#include <gtest/gtest.h>
#include <ghclass.hpp>


TEST(LoadRepos, individual)
{
     std::vector<Repo> repos = parse_file("./classroom_roster.csv","shanep-classroom","project1");
     EXPECT_EQ(repos.size(),2);
     EXPECT_TRUE(repos[0].repo_id() == "BSU-ShanePanter");
     EXPECT_TRUE(repos[1].repo_id() == "student1");
     EXPECT_TRUE(repos[0].repo_name() == "git@github.com:shanep-classroom/project1-BSU-ShanePanter");
     EXPECT_TRUE(repos[1].repo_name() == "git@github.com:shanep-classroom/project1-student1");
}

TEST(LoadRepos, group)
{
     std::vector<Repo> repos = parse_file("./classroom_roster_groups.csv","shanep-classroom","project1");
     EXPECT_EQ(repos.size(),4);
     EXPECT_TRUE(repos[0].repo_id() == "team-001");
     EXPECT_TRUE(repos[1].repo_id() == "team-001");
     EXPECT_TRUE(repos[2].repo_id() == "team-002");
     EXPECT_TRUE(repos[3].repo_id() == "team-002");
     EXPECT_TRUE(repos[0].repo_name() == "git@github.com:shanep-classroom/project1-team-001");
     EXPECT_TRUE(repos[1].repo_name() == "git@github.com:shanep-classroom/project1-team-001");
     EXPECT_TRUE(repos[2].repo_name() == "git@github.com:shanep-classroom/project1-team-002");
     EXPECT_TRUE(repos[2].repo_name() == "git@github.com:shanep-classroom/project1-team-002");
}

