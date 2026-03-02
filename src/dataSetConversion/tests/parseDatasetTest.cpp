#include <gtest/gtest.h>

#include "../convert.hpp"

class ConvertorTest : public ::testing::Test {
protected:
    Convertor convertor;
};

TEST_F(ConvertorTest, GetLineInfoTest) {

    std::string input = "rnbqkbnr";
    std::vector<int> expected_output = { -4, -2, -3, -5, -6, -3, -2, -4 };
    std::vector<int> output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "8";
    expected_output = { 0, 0, 0, 0, 0, 0, 0, 0 };
    output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "PPPPPPPP";
    expected_output = { 1, 1, 1, 1, 1, 1, 1, 1 };
    output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "3k4";;
    expected_output = { 0, 0, 0, -6, 0, 0, 0, 0 };
    output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "4K3";;
    expected_output = { 0, 0, 0, 0, 6, 0, 0, 0 };
    output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "rp3b2";
    expected_output = { -4, -1, 0, 0, 0, -3, 0, 0 };
    output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "2B3N1";
    expected_output = { 0, 0, 3, 0, 0, 0, 2, 0 };
    output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "1q5r";;
    expected_output = { 0, -5, 0, 0, 0, 0, 0, -4 };
    output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "5Q2";
    expected_output = { 0, 0, 0, 0, 0, 5, 0, 0 };
    output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "n1b1r1q1";
    expected_output = { -2, 0, -3, 0, -4, 0, -5, 0 };
    output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "1Bn1B3";
    expected_output = { 0, 3, -2, 0, 3, 0, 0, 0 };
    output = convertor.getLineInfo(input);
    EXPECT_EQ(output, expected_output);
}

TEST_F(ConvertorTest, GetCastlingInfoTest) {
    std::string input = "KQkq";
    std::vector<int> expected_output = { 6, 5, -6, -5 };
    std::vector<int> output = convertor.getCastlingInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "Kk";
    expected_output = { 6, 0, -6, 0 };
    output = convertor.getCastlingInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "Qq";
    expected_output = { 0, 5, 0, -5 };
    output = convertor.getCastlingInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "KQ";
    expected_output = { 6, 5, 0, 0 };
    output = convertor.getCastlingInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "kq";
    expected_output = { 0, 0, -6, -5 };
    output = convertor.getCastlingInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "Kq";
    expected_output = { 6, 0, 0, -5 };
    output = convertor.getCastlingInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "Qk";
    expected_output = { 0, 5, -6, 0 };
    output = convertor.getCastlingInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "-";
    expected_output = { 0, 0, 0, 0 };
    output = convertor.getCastlingInfo(input);
    EXPECT_EQ(output, expected_output);
}

TEST_F(ConvertorTest, GetEnPassantInfoTest) {
    std::string input = "e3";
    std::vector<int> expected_output = { 4, 3 };
    std::vector<int> output = convertor.getEnPassantInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "a6";
    expected_output = { 0, 6 };
    output = convertor.getEnPassantInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "h1";
    expected_output = { 7, 1 };
    output = convertor.getEnPassantInfo(input);
    EXPECT_EQ(output, expected_output);

    input = "-";
    expected_output = { -1, -1 };
    output = convertor.getEnPassantInfo(input);
    EXPECT_EQ(output, expected_output);
}

TEST_F(ConvertorTest, ParseDataSetLineTest) {
    std::string input = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 Check White";
    std::vector<int> expected_output = {
        -4, -2, -3, -5, -6, -3, -2, -4,
        -1, -1, -1, -1, -1, -1, -1, -1,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1,
        4, 2, 3, 5, 6, 3, 2, 4,
        1,
        6, 5, -6, -5,
        -1, -1,
        0, 1,
        1, 0, 0, 0, 0
    };
    std::vector<int> output = convertor.parseDataSetLine(input);
    EXPECT_EQ(output, expected_output);
}

TEST_F(ConvertorTest, ParseDataSetTest) {
    std::vector<std::string> input = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 Check White",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq e3 0 1 Check Black"
    };

    std::vector<std::vector<int>> expected_output = {
        {
            -4, -2, -3, -5, -6, -3, -2, -4,
            -1, -1, -1, -1, -1, -1, -1, -1,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1,
            4, 2, 3, 5, 6, 3, 2, 4,
            1,
            6, 5, -6, -5,
            -1, -1,
            0, 1,
            1, 0, 0, 0, 0
        },
        {
            -4, -2, -3, -5, -6, -3, -2, -4,
            -1, -1, -1, -1, -1, -1, -1, -1,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0 ,0,
            0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,
            1, 1, 1, 1, 1, 1, 1, 1,
            4, 2, 3, 5, 6, 3, 2, 4,
            -1,
            6, 5, -6, -5,
            4, 3,
            0, 1,
            0, 1, 0, 0, 0
        }
    };
    std::vector<std::vector<int>> output = convertor.parseDataSet(input);   
    EXPECT_EQ(output, expected_output);
}