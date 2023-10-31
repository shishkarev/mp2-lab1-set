// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Множество - реализация через битовые поля

#include <iostream>
#include "tbitfield.h"
#include "tset.h"

TSet::TSet(size_t mp) : bitField(mp) {
    maxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : bitField(s.bitField) {
    this->bitField = s.bitField;
    maxPower = s.maxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : bitField(bf) {
    maxPower=bf.getLength();
    bitField=bf;
}

TSet::operator TBitField()
{
    return this->bitField;
}

size_t TSet::getMaxPower() const // получить макс. к-во эл-тов
{
    return maxPower;
}

bool TSet::isMember(const uint elem) const // элемент множества?
{
    if (bitField.getBit(elem)){
        return true;
    }
    return false;
}

void TSet::insElem(const uint elem) // включение элемента множества
{
    bitField.setBit(elem);
}

void TSet::delElem(const uint elem) // исключение элемента множества
{
    bitField.clrBit(elem);
}

// теоретико-множественные операции
TSet& TSet::operator=(const TSet &s) // присваивание
{
    if(*this!=s){
        this->maxPower=s.maxPower;
        this->bitField=s.bitField;
    }
    return *this;
}

bool TSet::operator==(const TSet &s) const // сравнение
{
    if (this->bitField==s.bitField) return true;
    return false;
}

bool TSet::operator!=(const TSet &s) const // сравнение
{
    if (this->bitField==s.bitField) return false;
    return true;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    if(this->maxPower>s.maxPower){
        TSet res(maxPower);
        for (int i=0;i<maxPower;i++) if(bitField.getBit(i)) res.insElem(i);
        for (int i=0;i<s.maxPower;i++) if(s.bitField.getBit(i)) res.insElem(i);
        return res;
    }
    else{
        TSet res(s.maxPower);
        for (int i=0;i<s.maxPower;i++) if(s.bitField.getBit(i)) res.insElem(i);
        for (int i=0;i<maxPower;i++) if(bitField.getBit(i)) res.insElem(i);
        return res;
    }
}

TSet TSet::operator+(const uint elem) // объединение с элементом
{
    bitField.setBit(elem);
    return *this;
}

TSet TSet::operator-(const uint elem) // разность с элементом
{
    bitField.clrBit(elem);
    return *this;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int max;
    if(this->maxPower>s.maxPower) max=this->maxPower;
    else max=s.maxPower;
    TBitField res(max);
    res=this->bitField&s.bitField;
    return TSet(res);
}

TSet TSet::operator~() // дополнение
{
    TSet res(maxPower);
    res.bitField=~(this->bitField);
    return res;
}

// перегрузка ввода/вывода

std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    using namespace std;
    istr >> s.maxPower;
    istr >> s.bitField;
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
    ostr << s.bitField;
    return ostr;
}
