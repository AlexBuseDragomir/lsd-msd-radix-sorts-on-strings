/**
* @file LSD_MSD.cpp
* @brief This file will contain LSD and MSD sort implementations
*
* @author Bușe-Dragomir Alexandru
*
* @date 31/12/2017
*/


#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Element.h"
#include "LSD_MSD.h"

/**
* This is a predicate used in order to implement a STL sort on a vector<Element>
* @author Bușe-Dragomir Alexandru
* @param element1 The reference to the first Element object
* @param element2 The reference to the second Element object
* @date 31/12/2017
*/
bool compare(Element &element1, Element &element2)
{
    return (element1.getWord() < element2.getWord());
}

/**
* This is a STL sort implemented on a vector<Element> ////  O(n*log(n))
* @author Bușe-Dragomir Alexandru
* @param my_array The reference to a vector<Element> (a vector of Element objects)
* @param alphabet_size The size of the alphabet we are working on
* @date 31/12/2017
*/
void stl_quick_sort(vector<Element> &my_array, int alphabet_size)
{
    sort(my_array.begin(), my_array.end(), compare);
}

/**
* This is a selection sort implemented on a vector<Element> //// O(n^2)
* @author Bușe-Dragomir Alexandru
* @param my_array The reference to a vector<Element> (a vector of Element objects)
* @param alphabet_size The size of the alphabet we are working on
* @date 31/12/2017
*/
void selection_sort(vector<Element> &my_array, int alphabet_size)
{
    for(unsigned int i = 0; i < my_array.size() - 1; i ++)
    {
        for(unsigned int j = i + 1; j < my_array.size(); j ++)
        {
            if(my_array[i].getWord() >= my_array[j].getWord())
            {
                swap(my_array[i], my_array[j]);
            }
        }
    }
}

/**
* This is a key indexed counting sort implemented on a vector<Element> //// O(n + k), where k = alphabet_size
* @author Bușe-Dragomir Alexandru
* @param my_array The reference to a vector<Element> (a vector of Element objects)
* @param alphabet_size The size of the alphabet we are working on
* @date 31/12/2017
*/
void key_indexed_counting(vector<Element> &my_array, int alphabet_size)
{
    int dimension = my_array.size();

    int counter[alphabet_size + 1] = {0};

    for(auto it = my_array.begin(); it != my_array.end(); it ++)
    {
        counter[it -> getKey() + 1] ++;
    }

    for(int i = 0; i < alphabet_size; i ++)
    {
        counter[i + 1] = counter[i] + counter[i + 1];
    }

    Element empty_element;

    vector<Element> auxiliary_array(dimension, empty_element);

    for(int p = 0; p < dimension; p ++)
    {
        string current_word = my_array[p].getWord();
        char current_key = my_array[p].getKey();
        int current_position = counter[int(current_key)];

        auxiliary_array[current_position].setWord(current_word);
        auxiliary_array[current_position].setKey(current_key);

        counter[int(current_key)] ++;
    }

    for(int i = 0; i < dimension; i ++)
    {
        my_array[i].setKey(auxiliary_array[i].getKey());
        my_array[i].setWord(auxiliary_array[i].getWord());
    }
}

/**
* This is a least significant digit sort implemented on a vector<Element> //// O(n * k), where k = alphabet_size
* @author Bușe-Dragomir Alexandru
* @param my_array The reference to a vector<Element> (a vector of Element objects)
* @param alphabet_size The size of the alphabet we are working on
* @date 31/12/2017
*/
void LSD_sort(vector<Element> &my_array, int alphabet_size)
{
    string my_string = my_array.begin() -> getWord();
    size_t strings_size = my_string.length();

    for(int i = strings_size - 1; i >= 0; i --)
    {
        for(auto it = my_array.begin(); it != my_array.end(); it ++)
        {
            string my_word = it -> getWord();
            it -> setKey(my_word[i]);
        }

        key_indexed_counting(my_array, alphabet_size);
    }
}


/**
* This is a most significant digit sort implemented on a vector<Element> //// between O(n) and O(n * k), where k = alphabet_size
* @author Bușe-Dragomir Alexandru
* @param my_array The reference to a vector<Element> (a vector of Element objects)
* @param low This is the position in the vector of strings from which we sort
* @param high This is the position in the vector of strings up to which we sort (inclusive)
* @param position This is the position of the character from which we start our sort (0 is the first char, 1 is the second etc.)
* @param alphabet_size The size of the alphabet we are working on
* @date 31/12/2017
*/
void MSD_sort(vector<Element> &my_array, int low, int high, int position, int alphabet_size)
{

    int dimension = my_array.size();
    int counter[alphabet_size + 2] = {0};

    Element empty_element;
    vector<Element> auxiliary_array(dimension, empty_element);


    if(high <= low)
    {
        return;
    }

    else
    {
        for(int index = low; index <= high; index ++)
        {
            int char_index = my_array[index].getCharIndexAt(position);
            counter[char_index + 2] ++;
        }

        for(int i = 0; i < alphabet_size + 1; i ++)
        {
            counter[i + 1] = counter[i + 1] + counter[i];
        }

        for(int j = low; j <= high; j ++)
        {
            int char_index = my_array[j].getCharIndexAt(position);
            int current_position = counter[char_index + 1]++;
            auxiliary_array[current_position] = my_array[j];
        }

        for(int k = low; k <= high; k ++)
        {
            my_array[k] = auxiliary_array[k - low];
        }

        for(int t = 0; t < alphabet_size; t ++)
        {
            MSD_sort(my_array, low + counter[t], low + counter[t + 1] - 1, position + 1);
        }
    }
}
