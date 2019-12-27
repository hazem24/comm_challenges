/*
 * MACROS.h
 *
 * Created: 11/1/2019 5:31:29 PM
 *  Author: Zeyad
 */ 


#ifndef MACROS_H_
#define MACROS_H_

#define SETBIT(R,B) (R|=(1<<B))
#define CLRBIT(R,B) (R&=~(1<<B))
#define TGLBIT(R,B) (R^=(1<<B))
#define GETBIT(R,B) ((R&(1<<(B)))>>(B))



#endif /* MACROS_H_ */