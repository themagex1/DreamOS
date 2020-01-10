#pragma once

struct PageData
{
	int Frame;
	bool isFree;

	PageData();
	PageData(int Frame, bool isFree);
};