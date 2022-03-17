// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0) throw "Len<0";           
	BitLen = len;
	if (len % (8 * sizeof(TELEM)) == 0)
	{
		MemLen = len / (8 * sizeof(TELEM));
	}
	else
	{
		MemLen = len / (8 * sizeof(TELEM)) + 1;
	}

	pMem = new TELEM[MemLen];
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen) throw "Index < 0";
	return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0) throw "Mask < 0";
	int i = n % (8 * sizeof(TELEM));
	TELEM mask = 1 << i;															 //Битовый сдвиг на i позицию
	return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n > BitLen)) throw "Out of range";
	int indN = GetMemIndex(n);
	int maskN = GetMemMask(n);
	pMem[indN] = pMem[indN] | maskN;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n > BitLen)) throw "Out of range";
	int indN = GetMemIndex(n);
	int maskN = GetMemMask(n);
	pMem[indN] = pMem[indN] & ~maskN;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n > BitLen)) throw "Out of range";
	int indN = GetMemIndex(n);
	int maskN = GetMemMask(n);
	return pMem[indN] & maskN;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;
	if (bf.MemLen > MemLen)
	{
		delete[] pMem;
	}
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (this == &bf)
		return true;
	if (BitLen != bf.BitLen)
		return false;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return false;
	return true;	
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int len = BitLen;
	if (BitLen < bf.BitLen)
		len = bf.BitLen;
	TBitField tmp(len);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] = tmp.pMem[i] | bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len = BitLen;
	if (BitLen < bf.BitLen)
		len = bf.BitLen;
	TBitField tmp(len);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] = tmp.pMem[i] & bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание												 
{																								 
	//TBitField tmp(*this);																		 
	//for (int i = 0; i < BitLen; i++) 
	//{			
	//	if (tmp.GetBit(i))																		 
	//		tmp.ClrBit(i);																		 
	//	else																					 
	//		tmp.SetBit(i);																		 
	//}																							 
	//return tmp;

	int len = BitLen;
	TBitField tmp(len);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = ~pMem[i];
	int k = len % (sizeof(TELEM) * 8);
	int v = (1 << k) - 1;
	tmp.pMem[MemLen - 1] &= v;
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0;
	char c;																
	while (true) {																		
		istr >> c;																		
		if (c == '0')													
			bf.ClrBit(i++);																
		else if (c == '1')															
			bf.SetBit(i++);																
		else break;																		
	}																					
	return istr;																		
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.GetBit(i) == 1)
			ostr << '1';
		else ostr << '0';
	}
	return ostr;
}