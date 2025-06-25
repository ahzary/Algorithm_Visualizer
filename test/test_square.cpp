#include <gtest/gtest.h>
#include <QTest>
#include "square.h"

class squareTest : public testing::Test{
protected:
    square* entity = nullptr;

    void SetUp() override{
        entity = new square();
    }
    void TearDown() override{
        delete entity;
    }
};

TEST_F(squareTest, updateColor){
    entity->type = 1;
    entity->update_color();
    QBrush brush = entity->brush();

    EXPECT_EQ(brush.color(), QColor(Qt::white));
}
