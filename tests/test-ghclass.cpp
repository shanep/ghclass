#include <stdlib.h>
#include <lab.h>
#include <gtest/gtest.h>

static int *alloc_data(int i)
{
     int *rval = (int *)malloc(sizeof(int));
     *rval = i;
     return rval;
}

static void destroy_data(void *data)
{
     free(data);
}

static int compare_to(const void *a, const void *b)
{
     int fst = *(int *)a;
     int snd = *(int *)b;
     return fst - snd;
}

TEST(CreateAndDestroy, init)
{
     list_t *lst = nullptr;
     lst = list_init(destroy_data, compare_to);
     EXPECT_NE(lst, nullptr);
     EXPECT_NE(lst->head, nullptr);
     EXPECT_EQ(lst->size, 0);
     EXPECT_EQ(lst->head->data, nullptr);
     //Make sure the function pointers are pointing to the correct fuctions
     EXPECT_EQ(lst->destroy_data, destroy_data);
     EXPECT_EQ(lst->compare_to, compare_to);

     //Make sure we are a circular linked list
     EXPECT_NE(lst->head->next, nullptr);
     EXPECT_NE(lst->head->prev, nullptr);
     EXPECT_EQ(lst->head->next, lst->head->prev);

     list_destroy(&lst);
     EXPECT_EQ(lst, nullptr);
}

class ListWith0 : public testing::Test
{
protected:
     void SetUp() override
     {
          this->lst_ = list_init(destroy_data, compare_to);
     }

     void TearDown() override
     {
          list_destroy(&lst_);
     }

     list_t *lst_ = nullptr;
};

TEST_F(ListWith0, add1)
{
     list_add(lst_, alloc_data(1));
     EXPECT_EQ(lst_->size, 1);
     //With one node both next and prev should be equal
     EXPECT_EQ(lst_->head->next, lst_->head->prev);
     //Make sure we didn't clobber our sentinel node
     EXPECT_NE(lst_->head, lst_->head->next);
     EXPECT_NE(lst_->head, lst_->head->prev);
     EXPECT_EQ(lst_->head->data, nullptr);

     //Check to make sure our data actually made it into the node
     EXPECT_EQ(*((int *)lst_->head->next->data), 1);
     EXPECT_EQ(*((int *)lst_->head->prev->data), 1);
}

TEST_F(ListWith0, add2)
{
     list_add(lst_, alloc_data(1));
     EXPECT_EQ(lst_->size, 1);
     list_add(lst_, alloc_data(2));
     EXPECT_EQ(lst_->size, 2);

     //With two nodes both next and prev should NOT be equal
     EXPECT_NE(lst_->head->next, lst_->head->prev);
     //Make sure we didn't clobber our sentinel node
     EXPECT_NE(lst_->head, lst_->head->next);
     EXPECT_NE(lst_->head, lst_->head->prev);
     EXPECT_EQ(lst_->head->data, nullptr);

     //Check to make sure our next and prev have the correct data
     EXPECT_EQ(*((int *)lst_->head->next->data), 2);
     EXPECT_EQ(*((int *)lst_->head->prev->data), 1);
}

class ListWith5 : public testing::Test
{
protected:
     void SetUp() override
     {
          this->lst_ = list_init(destroy_data, compare_to);
          for (int i = 0; i < 5; i++)
          {
               list_add(lst_, alloc_data(i));
          }
          EXPECT_EQ(lst_->size, 5);
     }

     void TearDown() override
     {
          list_destroy(&lst_);
     }

     list_t *lst_ = nullptr;
};

TEST_F(ListWith5, removeIndex0)
{

     int *rval = (int *)list_remove_index(lst_, 0);
     EXPECT_EQ(lst_->size, 4);
     EXPECT_EQ(*rval, 4);
     free(rval);

     node_t *curr = lst_->head->next;
     //List should be 3->2->1->0
     for (int i = 3; i >= 0; i--)
     {
          EXPECT_EQ(*((int *)curr->data), i);
          curr = curr->next;
     }
     curr = lst_->head->prev;
     for (int i = 0; i <= 3; i++)
     {
          EXPECT_EQ(*((int *)curr->data), i);
          curr = curr->prev;
     }
}

TEST_F(ListWith5, removeIndex4)
{
     int *rval = (int *)list_remove_index(lst_, 4);
     EXPECT_EQ(lst_->size, 4);
     EXPECT_EQ(*rval, 0);
     free(rval);

     node_t *curr = lst_->head->next;
     //List should be 4->3->2->1
     for (int i = 3; i >= 0; i--)
     {
          EXPECT_EQ(*((int *)curr->data), i + 1);
          curr = curr->next;
     }
     curr = lst_->head->prev;
     for (int i = 0; i <= 3; i++)
     {
          EXPECT_EQ(*((int *)curr->data), i + 1);
          curr = curr->prev;
     }
}

TEST_F(ListWith5, removeIndex3)
{
     int *rval = (int *)list_remove_index(lst_, 3);
     EXPECT_EQ(lst_->size, 4);
     EXPECT_EQ(*rval, 1);
     free(rval);

     node_t *curr = lst_->head->next;
     //List should be 4->3->2->0
     for (int i = 3; i >= 1; i--)
     {
          EXPECT_EQ(*((int *)curr->data), i + 1);
          curr = curr->next;
     }
     //Check the last one
     EXPECT_EQ(*((int *)curr->data), 0);

     //Set the curr back one node so we can check prev links
     curr = curr->prev;
     for (int i = 1; i <= 3; i++)
     {
          EXPECT_EQ(*((int *)curr->data), i + 1);
          curr = curr->prev;
     }
}

TEST_F(ListWith5, invaidIndex)
{
     void *rval = list_remove_index(lst_, 666);
     EXPECT_EQ(rval, nullptr);
     EXPECT_EQ(lst_->size, 5);

     node_t *curr = lst_->head->next;
     //List should be 4->3->2->1->0
     for (int i = 4; i >= 0; i--)
     {
          EXPECT_EQ(*((int *)curr->data), i);
          curr = curr->next;
     }

     for (int i = 0; i >= 4; i++)
     {
          EXPECT_EQ(*((int *)curr->data), i);
          curr = curr->prev;
     }
}

TEST_F(ListWith5, removeAll)
{
     //List should be 4->3->2->1->0
     for (int i = 4; i >= 0; i--)
     {
          int *rval = (int *)list_remove_index(lst_, 0);
          EXPECT_EQ(*rval, i);
          free(rval);
     }

     //Make sure we back to default
     EXPECT_NE(lst_->head->next, nullptr);
     EXPECT_NE(lst_->head->prev, nullptr);
     EXPECT_EQ(lst_->head->next, lst_->head->prev);
     EXPECT_EQ(lst_->size, 0);
}

TEST_F(ListWith5, indexOf0)
{
     //List should be 4->3->2->1->0
     void *data = lst_->head->next->data;
     size_t idx = list_indexof(lst_, data);
     EXPECT_EQ(idx, 0);
}
TEST_F(ListWith5, indexOf3)
{
     //List should be 4->3->2->1->0
     void *data = alloc_data(1);
     size_t idx = list_indexof(lst_, data);
     EXPECT_EQ(idx, 3);
     free(data);
}

TEST_F(ListWith5, notInList)
{
     void *data = alloc_data(22);
     size_t idx = list_indexof(lst_, data);
     EXPECT_EQ(idx, -1);
     free(data);
}