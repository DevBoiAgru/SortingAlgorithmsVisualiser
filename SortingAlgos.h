#pragma once
#include <random>
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstring>
#include <SFML/graphics.hpp>
#include "Button.h"

// CONSTANTS
constexpr int WINDOW_X = 1080;
constexpr int WINDOW_Y = 720;
constexpr int SIDEOFFSET = 50;						// Offset from the edges of the screen
constexpr int MAX_BOGOSORT_ITERATIONS = 1000000;	// Maximum iterations allowed for bogosort

// SEMI-CONSTANT
int SORT_DELAY = 1;						// Delay between sorting operations (in nanoseconds)

std::vector<int> numbers;
std::vector<sf::RectangleShape> rectangles;
std::default_random_engine randomGen;

int highlight1{ -1 }, highlight2{ -1 }, highlight3{ -1 };
bool canClick{ true };

// Helper to check if a C string is a number
int is_number(const char* s) {
	if (*s == '-' || *s == '+') return 0;   // no sign please, it is understood it'll be positive only
	if (*s == '\0') return 0;          // contrary to popular belief empty string is not a number

	while (*s) {
		if (*s < '0' || *s > '9') return 0;
		s++;
	}
	return 1;
}


static void drawRectangles();
static void HighlightRectangles(int hl1, int hl2, int hl3) {
	highlight1 = hl1;
	highlight2 = hl2;
	highlight3 = hl3;
};

// Utility functions
int maxNumber(std::vector<int> numbers) {
	int maxNum = numbers[0];
	for (int i : numbers) {
		if (i > maxNum)
			maxNum = i;
	}
	return maxNum;
}

void swapNums(int* n1, int* n2) {
	int temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}

void resetHighlights() {
	highlight1 = highlight2 = highlight3 = -1;
}

// Sorting algorithms

bool isSorted(std::vector<int> nums){
	if (nums.size() == 0 || nums.size() == 1)
		return true;
	for (int i = 1; i < nums.size(); i++)
		if (nums[i - 1] > nums[i])
			return false;
	return true;
}

void Verify(std::vector<int>& nums) {
	for (int i = 0; i < nums.size(); i++) {
		HighlightRectangles(-1, -1, i);
		std::this_thread::sleep_for(std::chrono::nanoseconds(SORT_DELAY));
	}
	return;
}

void BubbleSort(std::vector<int>& nums) {
	for (int i = 0; i < nums.size(); i++) {
		for (int j = 0; j < nums.size() - i - 1; j++) {
			if (nums[j] > nums[j + 1])
				swapNums(&nums[j], &nums[j + 1]);
			HighlightRectangles(j, j+1, -1);
			std::this_thread::sleep_for(std::chrono::nanoseconds(SORT_DELAY));
		}
	}
	Verify(nums);
	return;
}

void InsertionSort(std::vector<int>& nums) {
	int tmpNum, minIndex;
	for (int i = 0; i < nums.size(); i++) {
		tmpNum = nums[i];
		minIndex = i;
		while (minIndex > 0 && tmpNum < nums[minIndex - 1]) {
			nums[minIndex] = nums[minIndex - 1];
			minIndex -= 1;
			HighlightRectangles(i, minIndex, -1);
			std::this_thread::sleep_for(std::chrono::nanoseconds(SORT_DELAY));
		}
		nums[minIndex] = tmpNum;
	}
	Verify(nums);
	return;
}

void StalinSort(std::vector<int>& nums){
	std::vector<int>::iterator i = nums.begin();
	std::vector<int>::iterator j = i;
	std::vector<int> gulag;
	while (i != nums.end()){
		if (*i < *j){
			gulag.push_back(*i);
			i = nums.erase(i);
            HighlightRectangles(static_cast<int>(j - nums.begin()), static_cast<int>(i - nums.begin()), -1);
		}
		else{
			j = i;
			i++;
            HighlightRectangles(-1, static_cast<int>(i - nums.begin()), static_cast<int>(j - nums.begin()));
		}
		std::this_thread::sleep_for(std::chrono::nanoseconds(SORT_DELAY));
	}
	Verify(nums);
	return;
}

int partition(std::vector<int>& nums, int low, int high) {
	int pivot = nums[high];
	int i = (low - 1);
	for (int j = low; j < high; j++) {
		if (nums[j] <= pivot) {
			i++;
			HighlightRectangles(-1, i, j);
			swapNums(&nums[i], &nums[j]);
		}
		else {
			HighlightRectangles(-1, i, j);
		}
		std::this_thread::sleep_for(std::chrono::nanoseconds(SORT_DELAY));
	}
	swapNums(&nums[i + 1], &nums[high]);
	return (i + 1);
}

void QuickSort(std::vector<int>& nums, int low, int high) {
	if (low < high) {
		int partIndex = partition(nums, low, high);
		QuickSort(nums, low, partIndex - 1);
		QuickSort(nums, partIndex + 1, high);
	}
	return;
}

void SelectionSort(std::vector<int>& nums) {
	int i, j, minId;
	for (i = 0; i < nums.size() - 1; i++) {
		minId = i;
		for (j = i + 1; j < nums.size(); j++) {
			if (nums[j] < nums[minId]) {
				minId = j;
				HighlightRectangles(j, i, -1);
			}
			else {
				HighlightRectangles(-1, i, j);
			}
			std::this_thread::sleep_for(std::chrono::nanoseconds(SORT_DELAY));
		}
		if (minId != i)
			swapNums(&nums[minId], &nums[i]);
	}
	Verify(nums);
	return;
}

void BogoSort(std::vector<int>& nums) {
	int counter = 0;
	while (counter < MAX_BOGOSORT_ITERATIONS) {
		if (!isSorted(nums)) {
			std::shuffle(nums.begin(), nums.end(), randomGen);
			counter++;
		}
		else {
			Verify(nums);
			break;
		}
	}
	return;
}