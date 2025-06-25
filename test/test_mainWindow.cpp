#include <gtest/gtest.h>
#include "mainwindow.h"
#include <QTest>
// Test fixture for MainWindow
class MainWindowTest : public testing::Test {
protected:
    MainWindow* window = nullptr;

    void SetUp() override {
        window = new MainWindow();
        //window->show();
    }

    void TearDown() override {
        delete window;
    }
};
// Basic test: check if map pointer is initialized
TEST_F(MainWindowTest, MapIsInitialized) {
    ASSERT_NE(window->Map, nullptr);
}

TEST_F(MainWindowTest, SceneAdd) {
    QGraphicsScene* scene = window->getScene(); // expose it with a getter
    ASSERT_NE(scene, nullptr);
    int initialItemCount = scene->items().size();
    // Add a test item to the scene
    scene->addRect(0, 0, 10, 10);
    ASSERT_GT(scene->items().size(), 0);

    // Expect the scene to be not empty
    EXPECT_EQ(scene->items().size(),initialItemCount + 1);
}
