#include <gtest/gtest.h>
#include <QTest>
#include "gmap.h"

class mapTest : public testing::Test{
protected:
    GMap* entity = nullptr;

    int numOfSquare = 10;
    int squareSize = 5;

    void SetUp() override{
        entity = new GMap(numOfSquare,squareSize);
    }
    void TearDown() override{
        delete entity;
    }
};

TEST_F(mapTest, GridSize){
    entity->resize(25,squareSize);

   ASSERT_EQ(entity->getGridSize() ,25);
}

TEST_F(mapTest, squareSize){
    entity->setSquareSize(15);

    EXPECT_EQ(entity->getSquareSize() ,15);
}

TEST_F(mapTest, reseting){
    // Change some squares to simulate use
    entity->type_grid[0][0] = 2;
    entity->type_grid[1][1] = 3;

    entity->backup_map();  // back up the map

    // Now modify the map
    entity->Grid[0][0]->type = 4;
    entity->Grid[1][1]->type = 4;

    // Reset to backup
    entity->reset_map();

    // Check if it restored the original types
    EXPECT_EQ(entity->Grid[0][0]->type, 2);
    EXPECT_EQ(entity->Grid[1][1]->type, 3);
    EXPECT_NE(entity->Grid[0][0]->type, 4);
    EXPECT_NE(entity->Grid[1][1]->type, 4);
}
