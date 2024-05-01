#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include "Button.h"

// CONSTANTS
constexpr int WINDOW_X = 1920;
constexpr int WINDOW_Y = 1080;
constexpr int SIDEOFFSET = 50;					// Offset from the edges of the screen
constexpr int SORT_DELAY = 1;					// Delay between sorting operations (in microseconds)
constexpr int MAX_BOGOSORT_ITERATIONS = 2500;	// Maximum iterations allowed for bogosort
std::vector<int> numbers;
std::vector<sf::RectangleShape> rectangles;
std::default_random_engine randomGen;

static void drawRectangles(int highlight1, int highlight2, int highlight3, bool display);

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
		drawRectangles(-1, -1, i, true);
		std::this_thread::sleep_for(std::chrono::microseconds(SORT_DELAY));
	}
}

void BubbleSort(std::vector<int>& nums) {
	for (int i = 0; i < nums.size(); i++) {
		for (int j = 0; j < nums.size() - i - 1; j++) {
			if (nums[j] > nums[j + 1])
				swapNums(&nums[j], &nums[j + 1]);
			drawRectangles(j, j+1, -1, true);
			std::this_thread::sleep_for(std::chrono::microseconds(SORT_DELAY));
		}
	}
	Verify(nums);
}

void InsertionSort(std::vector<int>& nums) {
	int tmpNum, minIndex;
	for (int i = 0; i < nums.size(); i++) {
		tmpNum = nums[i];
		minIndex = i;
		while (minIndex > 0 && tmpNum < nums[minIndex - 1]) {
			nums[minIndex] = nums[minIndex - 1];
			minIndex -= 1;
			drawRectangles(i, minIndex, -1, true);
			std::this_thread::sleep_for(std::chrono::microseconds(SORT_DELAY));
		}
		nums[minIndex] = tmpNum;
	}
	Verify(nums);
}

void StalinSort(std::vector<int>& nums){
	std::vector<int>::iterator i = nums.begin();
	std::vector<int>::iterator j = i;
	std::vector<int> gulag;
	while (i != nums.end()){
		if (*i < *j){
			gulag.push_back(*i);
			i = nums.erase(i);
			drawRectangles(j - nums.begin(), i - nums.begin(), -1, true);
		}
		else{
			j = i;
			i++;
			drawRectangles(-1, i - nums.begin(), j - nums.begin(), true);
		}
		std::this_thread::sleep_for(std::chrono::microseconds(SORT_DELAY));
	}
	Verify(nums);
}

int partition(std::vector<int>& nums, int low, int high) {
	int pivot = nums[high];
	int i = (low - 1);
	for (int j = low; j < high; j++) {
		if (nums[j] <= pivot) {
			i++;
			drawRectangles(-1, i, j, true);
			swapNums(&nums[i], &nums[j]);
		}
		else {
			drawRectangles(-1, i, j, true);
		}
		std::this_thread::sleep_for(std::chrono::microseconds(SORT_DELAY));
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
}

void SelectionSort(std::vector<int>& nums) {
	int i, j, minId;
	for (i = 0; i < nums.size() - 1; i++) {
		minId = i;
		for (j = i + 1; j < nums.size(); j++) {
			if (nums[j] < nums[minId]) {
				minId = j;
				drawRectangles(j, i, -1, true);
			}
			else {
				drawRectangles(-1, i, j, true);
			}
			std::this_thread::sleep_for(std::chrono::microseconds(SORT_DELAY));
		}
		if (minId != i)
			swapNums(&nums[minId], &nums[i]);
	}
}

void BogoSort(std::vector<int>& nums) {
	int counter = 0;
	while (counter < MAX_BOGOSORT_ITERATIONS) {
		if (!isSorted(nums)) {
			std::shuffle(nums.begin(), nums.end(), randomGen);
			drawRectangles(-1, -1, -1, true);
			counter++;
		}
		else {
			Verify(nums);
			break;
		}
	}
}