/*
Author
Nama 	: Dery Rahman A
NIM		: 13515097
Tanggal	: 07/11/2016
File	: tree.c
*/

#include <stdio.h>
#include "tree.h"

Tree TreeCreate(){
	return TreeAlloc(SkillCreate(0,0,0,0,StringCreate("")));
}
TreeAddress TreeAlloc(Skill X){
	TreeAddress P=(TreeAddress) malloc(sizeof(Node));
	if(P!=Nil){
		TreeRoot(P)=X;
		TreeLeft(P)=Nil;
		TreeRight(P)=Nil;
		TreeStatus(P)=false;
	}
	return P;
}
void TreeDealloc(TreeAddress P){
	free(P);
}
boolean TreeIsEmpty(Tree P){
	return P==Nil;
}
boolean TreeIsOneElmt(Tree P){
	if (!(TreeIsEmpty(P))){
		return (TreeLeft(P)==Nil) && (TreeRight(P)==Nil);
	} else {
		return false;
	}
}
void TreeAddLeft(Tree P, Skill X, TreeAddress *Pl){
	TreeAddress PLeft=TreeAlloc(X);
	TreeLeft(P)=PLeft;
	*Pl=PLeft;
}
void TreeAddRight(Tree P, Skill X, TreeAddress *Pr){
	TreeAddress PRight=TreeAlloc(X);
	TreeRight(P)=PRight;
	*Pr=PRight;
}

void TreePrint(Tree P, int curr){
	if(P==Nil){}
	if ((TreeLeft(P) == Nil) || (TreeRight(P) == Nil)) {
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|-------------------- Min EXP : %u\n", TreeRoot(P).lvl_req);
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|- Att : ++%u\n",TreeRoot(P).attack);
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|- Def : ++%u\n",TreeRoot(P).deffense);
	} else {
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|-------------------- Min EXP : %u\n", TreeRoot(P).lvl_req);
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|- Att : ++%u\n",TreeRoot(P).attack);
		for(int i=0;i<curr;i++){
			printf("|   ");
		}
		printf("|- Def : ++%u\n",TreeRoot(P).deffense);
		TreePrint(TreeLeft(P),curr+1);
		TreePrint(TreeRight(P),curr+1);
	}
}

void TreePrintActivated(Tree P, int curr){
	if (P == Nil)
		return;
	if (TreeStatus(P)==true){
		if ((TreeLeft(P) == Nil || !TreeStatus(TreeLeft(P))) && (TreeRight(P) == Nil || !TreeStatus(TreeRight(P)))) {
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|-------------------- Min EXP : %u\n", TreeRoot(P).lvl_req);
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|- Att : ++%u\n",TreeRoot(P).attack);
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|- Def : ++%u\n",TreeRoot(P).deffense);
		} else {
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|-------------------- Min EXP : %u\n", TreeRoot(P).lvl_req);
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|- Att : ++%u\n",TreeRoot(P).attack);
			for(int i=0;i<curr;i++){
				printf("|   ");
			}
			printf("|- Def : ++%u\n",TreeRoot(P).deffense);
			TreePrintActivated(TreeLeft(P),curr+1);
			TreePrintActivated(TreeRight(P),curr+1);
		}
	}
}

uint SkillTotalAttack(Tree P){
	if (TreeStatus(P)==true){
		if ((TreeLeft(P) == Nil || !TreeStatus(TreeLeft(P))) && (TreeRight(P) == Nil || !TreeStatus(TreeRight(P)))){
			return TreeRoot(P).attack;
		} else {
			return TreeRoot(P).attack+SkillTotalAttack(TreeLeft(P))+SkillTotalAttack(TreeRight(P));
		}
	}
	return 0;
}

uint SkillTotalDeffense(Tree P){
	if (TreeStatus(P)==true){
		if ((TreeLeft(P) == Nil || !TreeStatus(TreeLeft(P))) && (TreeRight(P) == Nil || !TreeStatus(TreeRight(P)))){
			return TreeRoot(P).deffense;
		} else {
			return TreeRoot(P).deffense+SkillTotalDeffense(TreeLeft(P))+SkillTotalDeffense(TreeRight(P));
		}
	}
	return 0;
}

void SkillActivatedGenerate(Tree *P, int LVL){
	if (P != Nil && *P != Nil && LVL >= TreeRoot(*P).lvl_req) {
		TreeStatus(*P)=true;
		SkillActivatedGenerate(&TreeLeft(*P),LVL);
		SkillActivatedGenerate(&TreeRight(*P),LVL);
	}
}

void SkillTreeDefault(Tree *Pt){
	TreeAddress P,PChild;
	Tree root=TreeAlloc(SkillCreate(1,1,5,0, StringCreate("Rinnegan")));
	// Left
	TreeAddLeft(root,SkillCreate(3,1,10,1, StringCreate("Teigu")),&P);
		// Left Left
		TreeAddLeft(P,SkillCreate(4,2,20,3, StringCreate("Kagune")),&PChild);
		// Left Right
		TreeAddRight(P,SkillCreate(2,4,30,4, StringCreate("Alchemy")),&PChild);

	// Right
	TreeAddRight(root,SkillCreate(1,3,15,2, StringCreate("Titan Shift")),&P);
		// Left Left
		TreeAddLeft(P,SkillCreate(3,3,40,5, StringCreate("Death Note")),&PChild);
		// Left Right
		TreeAddRight(P,SkillCreate(3,3,50,6, StringCreate("Super Saiyan")),&PChild);
	*Pt=root;
}

void LoadSkill(Tree *P, int LVL){
	SkillTreeDefault(P);
	SkillActivatedGenerate(P,LVL);
}
