// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include <exception>
#include <vector>
#include "tbitfield.h"

typedef size_t elType;

class Error : private std::exception{

};

TBitField::TBitField(size_t len)
{
    bitLen=len;
    if (len==0) memLen=0;
    else if (len>0) memLen=1+len/(sizeof(elType)*8);
    this->pMem = new elType[memLen];
    for (size_t i=0; i<memLen; i++ ){
        this->pMem[i]=0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    this->bitLen=bf.bitLen;
    this->memLen=bf.memLen;
    this->pMem = new elType[memLen];
    for (size_t i=0; i<memLen; i++){
        this->pMem[i]=bf.pMem[i];
    }
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    return (n/(sizeof(elType)*8));
}

elType TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    elType tmp = 0;
    tmp=tmp|1<<(n%(sizeof(elType)*8));
    return tmp;
}

// доступ к битам битового поля
elType TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

size_t TBitField::getNumBytes() const // получить количество байт выделенной памяти
{
    if (bitLen==0) return memLen * sizeof(elType);
    else return memLen * sizeof(elType)-8;
}

void TBitField::setBit(const size_t n) // установить бит
{
    if (n<bitLen){
        this->pMem[getIndex(n)] = this->pMem[getIndex(n)] | getMask(n);
    }
    else if (n>=bitLen){
        Error falt;
        throw falt;
    }
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    if (n<bitLen){
        this->pMem[getIndex(n)] = this->pMem[getIndex(n)] & ~(getMask(n));
    }
    else if ((n>=bitLen) || (n<0)){
        Error falt;
        throw falt;
    }
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    if ((n>bitLen) || (n<0)){
        Error falt;
        throw falt;
    }
    else {
        if (getMask(n)&pMem[getIndex(n)]){
            return true;
        }
        else return false;
    }
    return false;
}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    delete[]pMem;
    bitLen=bf.bitLen;
    memLen=bf.memLen;
    pMem=new elType[memLen];
    for (int i=0;i<memLen;i++){
        pMem[i]=bf.pMem[i];
    }
    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this->bitLen!=bf.bitLen) return false;
    else{
        for(int i=0; i<this->memLen;i++){
            if(pMem[i]!=bf.pMem[i]) return false;
        }
    }
    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (*this==bf) return false;
    else return true;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (this->bitLen==bf.bitLen){
        TBitField res(bitLen);
        for(int i=0; i<bitLen;i++){
            if(this->getBit(i)==1 || bf.getBit(i)==1){
                res.setBit(i);
            }
        }
        return res;
    }
    else if (this->bitLen>bf.bitLen){
        TBitField res(bitLen);
        for(int i=0; i<bf.bitLen;i++){
            if(this->getBit(i)==1 || bf.getBit(i)==1){
                res.setBit(i);
            }
        }
        for(int i=bf.bitLen; i<this->bitLen;i++){
            if(this->getBit(i)==1){
                res.setBit(i);
            }
        }
        return res;
    }
    else{
        TBitField res(bf.bitLen);
        for(int i=0; i<this->bitLen;i++){
            if(this->getBit(i)==1 || bf.getBit(i)==1){
                res.setBit(i);
            }
        }
        for(int i=this->bitLen; i<bf.bitLen;i++){
            if(this->getBit(i)==1){
                res.setBit(i);
            }
        }
        return res;
    }
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (this->bitLen==bf.bitLen){
        TBitField res(bitLen);
        for(int i=0; i<bitLen;i++){
            if(this->getBit(i)==1 && bf.getBit(i)==1){
                res.setBit(i);
            }
        }
        return res;
    }
    else{
        int max;
        int min;
        if (this->bitLen>bf.bitLen){
            max=this->bitLen;
            min=bf.bitLen;
        }
        else{
            max=bf.bitLen;
            min=this->bitLen;
        }
        TBitField res(max);
        for(int i=0; i<min;i++){
            if(this->getBit(i)==1 && bf.getBit(i)==1){
                res.setBit(i);
            }
        }
        return res;
    }
}

TBitField TBitField::operator~() // отрицание
{
    TBitField res(bitLen);
    for (int i=0;i<memLen-1;i++){
        res.pMem[i]=~(this->pMem[i]);
    }
    for(int i=8*sizeof(elType)*(memLen-1);i<bitLen;i++){
        if(getBit(i)==0) res.setBit(i);
    }
    return res;
}

TBitField::~TBitField()
{
    delete[]pMem;
}

// ввод/вывод
std::istream &operator>>(std::istream &istr, TBitField &bf) // ввод
{
    std::cin>>bf.bitLen;
    if(bf.bitLen==0) bf.memLen=0;
    else if(bf.bitLen<0){
        Error falt;
        throw falt;
    }
    else bf.memLen=bf.bitLen/(sizeof(elType)*8)+1;
    bf.pMem=new elType[bf.memLen];
    for (int i=0;i<bf.memLen;i++) istr >> bf.pMem[i];
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) // вывод
{
    for (int i=0; i<bf.memLen-1;i++){
        elType tmp = (elType)bf.pMem[i];
        elType size=sizeof(elType)*8;
        std::string str = "";
        for (int j=0;j<size;j++){
            str = std::to_string(tmp%2)+str;
            tmp/=2;
        }
        ostr << str;
    }
    return ostr;
}