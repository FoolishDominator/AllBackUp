#include <gtest/gtest.h>
#include "FileManager.h"
#include "../src/FileManager.cpp"

TEST(PACK_TEST, readdir)
{
    {
        FileManager *fm = new (FileManager);
        fm->read_dir("../testdir");
        File *file;
        // 检测是否读取到特定文件
        bool flag[9] = {0};

        while ((file = fm->get_file()) != NULL)
        {
            if (file->get_name() == "a")
            {
                flag[0] = 1;
            }
            if (file->get_name() == "b")
            {
                flag[1] = 1;
            }
            if (file->get_name() == "hl_abc2")
            {
                flag[2] = 1;
            }
            if (file->get_name() == "subdir")
            {
                flag[3] = 1;
            }
            if (file->get_name() == "abc")
            {
                flag[4] = 1;
            }
            if (file->get_name() == "hl_abc")
            {
                flag[5] = 1;
            }
            if (file->get_name() == "sl_a")
            {
                flag[6] = 1;
            }
            if (file->get_name() == "sl_abc")
            {
                flag[7] = 1;
            }
            if (file->get_name() == "sl_readme")
            {
                flag[8] = 1;
            }
            fm->next_file();
        }

        EXPECT_EQ(flag[0], 1);
        EXPECT_EQ(flag[1], 1);
        EXPECT_EQ(flag[2], 1);
        EXPECT_EQ(flag[3], 1);
        EXPECT_EQ(flag[4], 1);
        EXPECT_EQ(flag[5], 1);
        EXPECT_EQ(flag[6], 1);
        EXPECT_EQ(flag[7], 1);
        EXPECT_EQ(flag[8], 1);
        free(fm);
    }
}

TEST(PACK_TEST, fileType)
{
    {
        FileManager *fm = new (FileManager);
        fm->read_dir("../testdir");
        File *file;
        // 检测读取的元数据是否正确

        while ((file = fm->get_file()) != NULL)
        {
            if (file->get_name() == "a")
            {

                EXPECT_EQ(file->get_filetype(), FILETYPE_REG);
            }
            if (file->get_name() == "b")
            {
                EXPECT_EQ(file->get_filetype(), FILETYPE_REG);
            }
            if (file->get_name() == "hl_abc2")
            {
                EXPECT_EQ(file->get_filetype(), FILETYPE_HARDLNK);
            }
            if (file->get_name() == "subdir")
            {
                EXPECT_EQ(file->get_filetype(), FILETYPE_DIR);
            }
            if (file->get_name() == "abc")
            {
                EXPECT_EQ(file->get_filetype(), FILETYPE_REG);
            }
            if (file->get_name() == "hl_abc")
            {
                EXPECT_EQ(file->get_filetype(), FILETYPE_HARDLNK);
            }
            if (file->get_name() == "sl_a")
            {
                EXPECT_EQ(file->get_filetype(), FILETYPE_SYMLNK);
            }
            if (file->get_name() == "sl_abc")
            {
                EXPECT_EQ(file->get_filetype(), FILETYPE_SYMLNK);
            }
            if (file->get_name() == "sl_readme")
            {
                EXPECT_EQ(file->get_filetype(), FILETYPE_SYMLNK);
            }
            fm->next_file();
        }
        free(fm);
    }
}
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}