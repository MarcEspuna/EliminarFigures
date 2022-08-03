#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <unordered_map>

#define MAX_X 550.0f			// In reality it's 640 but we add margin
#define MAX_Y 400.0f			// In reality it's 480 but we add margin
#define BOT_MARGIN 140.0f		// Added margin in bottom of the screen

/* ROWS RANGE IS FROM [0-4] AND COLUMNS RANGE IS FROM [0-6] */

struct Row {

	Row(unsigned int idxRow)
		:row(idxRow), xSpacing((MAX_X * 2.0f) / 6.0f), ySpacing((MAX_Y * 2.0f - BOT_MARGIN) / 5.0f) {}

	// Operator [] in order to 
	glm::mat4 operator[](unsigned int column) {
		float xOrigin = - MAX_X;
		float yOrigin = - MAX_Y + BOT_MARGIN;
		xOrigin += xSpacing * column;
		yOrigin += ySpacing * row;
		return glm::translate(glm::mat4(1.0f), glm::vec3(xOrigin, yOrigin, 0.0f));
	}


private:

	const unsigned int row;			// In which row is this instance assigned to
	const float xSpacing;			// Spacing between each column 
	const float ySpacing;			// Spacing between each column

};



struct Slot
{

	Slot() {
		clear();
	}

	//30 slots in total, 5 rows and 6 columns
	Row& operator[](unsigned int idxRow)
	{
		Row* auxRow = &rows[idxRow];
		auto finding = std::find(std::begin(emptyRows), std::end(emptyRows), auxRow);
		if (finding != std::end(emptyRows)) {
			emptyRows.erase(finding);
		}
		return *auxRow;
	}

	// Resets all the rows and columns as empty
	void clear()
	{
		emptyRows.clear();				// Reaload the empty rows vector
		for (auto& row : rows) {
			emptyRows.push_back(&row);
		}
		emptyColumns.clear();			// Reload the emoty columns vector
		for (int i = 0; i < 7; i++) {
			emptyColumns.push_back(i);
		}
	}
	
	inline unsigned int getSize() const { return(unsigned int)emptyRows.size(); }							// Number of empty rows
	inline unsigned int getSizeCol() const { return(unsigned int)emptyColumns.size(); }						// Number of empty columns

	// Returns an avalable row and marks that row as ocuped
	Row& popRow(unsigned int indx) {
		Row* retRow = emptyRows[indx];
		emptyRows.erase(emptyRows.begin() + indx);

		return *retRow;
	}

	// Returns an avaliable model matrix slot and marks that slot as ocuped
	glm::mat4 getModel(unsigned int rowNumber, unsigned int colNumber) {
		Row& currentRow = popRow(rowNumber);						// We pop the row from the empty rows vector
		unsigned int idxColumn = emptyColumns[colNumber];			// We get the index of the selected colNumber
		emptyColumns.erase(emptyColumns.begin() + colNumber);		// We delete the index of the selected col from the empty columns vector											// Reduce the size of the available 
		return currentRow[idxColumn];
	}

private:
	// Main array where we will store all the avalable rows and columns
	Row rows[5] = {
		Row(0), Row(1), Row(2), Row(3), Row(4)
	};

	std::vector<Row*> emptyRows;					// We will keep track of the available rows in this vector
	std::vector<unsigned int> emptyColumns;			// We will keep track of the available column indeces in this vector
		
};


