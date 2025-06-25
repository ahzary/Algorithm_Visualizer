#include <gtest/gtest.h>
#include <QApplication>

// Main entry point
int main(int argc, char **argv) {
    QApplication app(argc, argv);  // Initialize Qt application
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
