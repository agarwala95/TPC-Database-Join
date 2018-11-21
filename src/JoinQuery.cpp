#include "../include/JoinQuery.hpp"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include<unordered_set>
#include <sstream>
#include <string>
//---------------------------------------------------------------------------
using namespace std;
using std::unordered_map;

std::string lPath,oPath,cPath;

std::unordered_map<size_t,std::string> cust_map ;
std::unordered_map<size_t,size_t> ord_map ;


double sum_mach=0;
double sum_auto=0;
double sum_furn=0;
double sum_house=0;
double sum_build=0;

uint64_t count_mach=0;
uint64_t count_auto=0;
uint64_t count_furn=0;
uint64_t count_house=0;
uint64_t count_build=0;

JoinQuery::JoinQuery(std::string lineitem, std::string order,
                     std::string customer)
{
    lPath = lineitem;
    oPath = order ;
    cPath = customer;

    //ensuring file is not read everytime the constructor is called
    if (cust_map.size()==0)
    {
        //Read all the files and store values in a map so that we don't read again
        size_t test = initLinetable();
    }
}

size_t JoinQuery::initLinetable()
{
    std::ifstream relation(cPath);
    size_t cust_ID=0;
    size_t word_count = 0;
    for (std::string line; std::getline(relation, line,'|');)
    {
        if(relation.eof())
        {
            break;
        }
        if(word_count == 0)
        {
            cust_ID=std::stoi(line);
        }
        else if(word_count == 6)
        {
            cust_map[cust_ID]=line;
        }
        else if(word_count == 7)
        {
            word_count = 0;
            continue;
        }
        word_count++;
    }

    std::ifstream relation_o(oPath);
    size_t ord_ID=0;
    size_t word_count_o = 0;

    for (std::string line_o; std::getline(relation_o,line_o);)
        for (std::string line; std::getline(relation_o, line,'|');)
        {
            if(relation_o.eof())
            {
                break;
            }
            if(word_count_o == 0)
            {
                ord_ID=std::stoi(line);
            }
            else if(word_count_o == 1)
            {
                ord_map[ord_ID]=std::stoi(line);
                word_count_o = 0;
                break;
            }
            word_count_o++;
        }

    std::ifstream relation_l(lPath);
    size_t ord_ID_l;
    size_t word_count_l = 0;

    for(std::string line_l;std::getline(relation_l,line_l);)
        for (std::string line; std::getline(relation_l, line,'|');)
        {
            if(relation_l.eof())
            {
                break;
            }
            if(word_count_l == 0)
            {
                ord_ID_l = std::stoi(line);
            }
            else if (word_count_l == 4)
            {
                if (cust_map[ord_map[ord_ID_l]]=="MACHINERY")
                {
                    sum_mach+= std::stoi(line);
                    count_mach+=1;
                }
                else if (cust_map[ord_map[ord_ID_l]]=="FURNITURE")
                {
                    sum_furn+= std::stoi(line);
                    count_furn+=1;
                }
                else if (cust_map[ord_map[ord_ID_l]]=="AUTOMOBILE")
                {
                    sum_auto+= std::stoi(line);
                    count_auto+=1;
                }
                else if (cust_map[ord_map[ord_ID_l]]=="BUILDING")
                {
                    sum_build+= std::stoi(line);
                    count_build+=1;
                }
                else
                {
                    sum_house+= std::stoi(line);
                    count_house+=1;
                }
                word_count_l = 0;
                break;
            }
            word_count_l++;
        }
    return 1;
}
//---------------------------------------------------------------------------
size_t JoinQuery::avg(std::string segmentParam)
{
    if (segmentParam=="MACHINERY")
    {
        return (sum_mach*100)/count_mach;
    }
    else if (segmentParam=="FURNITURE")
    {
        return (sum_furn*100)/count_furn;
    }
    else if (segmentParam=="AUTOMOBILE")
    {
        return (sum_auto*100)/count_auto;
    }
    else if (segmentParam=="BUILDING")
    {
        return (sum_build*100)/count_build;
    }
    else
    {
        return (sum_house*100)/count_house;
    }
}
//---------------------------------------------------------------------------
size_t JoinQuery::lineCount(std::string rel)
{
    std::ifstream relation(rel);
    assert(relation);  // make sure the provided string references a file
    size_t n = 0;
    for (std::string line; std::getline(relation, line);) n++;
    return n;
}
//---------------------------------------------------------------------------
