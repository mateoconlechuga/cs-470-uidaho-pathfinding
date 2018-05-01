/* prolog family tree for game of thrones  */
/* matt waltz  */
/* spring 2018  */

/* house lannister */

parent(tywin, jaime).
parent(tywin, cersei).
parent(tywin, tyrion).

parent(joanna, jaime).
parent(joanna, cersei).
parent(joanna, tyrion).

parent(jaime, joffrey).
parent(jaime, myrcella).
parent(jaime, tommen).

parent(cersei, joffrey).
parent(cersei, myrcella).
parent(cersei, tommen).

/* house baratheon */

parent(ormund, steffon).
parent(rhaelle, steffon).

parent(steffon, robert).
parent(steffon, stannis).
parent(steffon, renly).

parent(stannis, shireen).

/* house targaryen */

parent(maekar, aemon).
parent(maekar, aegon).

parent(aegon, jaehaerys).
parent(aegon, rhaelle).

parent(jaehaerys, aerys).

parent(aerys, rhaegar).
parent(aerys, viserys).
parent(aerys, daenerys).

parent(rhaegar, snow).
parent(lyanna, snow).

/* house stark */

parent(rickard, brandon).
parent(rickard, eddard).
parent(rickard, benjen).
parent(rickard, lyanna).

parent(eddard, robb).
parent(eddard, sansa).
parent(eddard, arya).
parent(eddard, brandon).
parent(eddard, rickon).

/* house tully */

parent(robbyrt, hoster).
parent(robbyrt, brynden).

parent(hoster, lysa).
parent(hoster, edmure).
parent(hoster, catelyn).

parent(minisa, lysa).
parent(minisa, edmure).
parent(minisa, catelyn).

parent(catelyn, robb).
parent(catelyn, sansa).
parent(catelyn, arya).
parent(catelyn, brandon).
parent(catelyn, rickon).

parent(lysa, robin).

/* house tyrell */

parent(luthor, mace).

parent(olenna, mace).

parent(mace, margaery).
parent(mace, loras).

/* house martell */

parent(oberyn, elia).

parent(doran, elia).

/* house arryn */

parent(jon, robin).

/* male - female */

male(jon).
male(robin).
male(oberyn).
male(doran).
male(luthor).
male(mace).
male(loras).
male(robbyrt).
male(hoster).
male(bryden).
male(edmure).
male(rickard).
male(brandon).
male(eddard).
male(benjen).
male(robb).
male(brandon).
male(rickon).
male(ormund).
male(steffon).
male(robert).
male(stannis).
male(renly).
male(tywin).
male(jaime).
male(tyrion).
male(joffrey).
male(tommen).
male(maekar).
male(aemon).
male(aegon).
male(jaehaerys).
male(aerys).
male(rhaegar).
male(viserys).
male(snow).
female(rhaelle).
female(daenerys).
female(shireen).
female(myrcella).
female(cersei).
female(joanna).
female(sansa).
female(arya).
female(lyanna).
female(catelyn).
female(lysa).
female(minisa).
female(elia).
female(olenna).
female(margaery).

/* rule defines */

child(A, B) :-
    parent(B, A).

sibling(A, B) :-
    parent(C, A),
    parent(C, B),
    A \= B.

related(A, B) :-
    ancestor(C, A),
    ancestor(C, B).

descendant(A, B) :-
    ancestor(B, A).

ancestor(A, B) :-
    parent(A, B).

ancestor(A, B) :-
    parent(C, B),
    ancestor(A, C).

mother(A, B) :-
    parent(A, B),
    female(A).

father(A, B) :-
    parent(A, B),
    male(A).

son(A, B) :-
    child(A, B),
    male(A).

daughter(A, B) :-
    child(A, B),
    female(A).

sister(A, B) :-
    sibling(A, B),
    female(A),
    A \= B.

brother(A, B) :-
    sibling(A, B),
    male(A),
    A \= B.

grandparent(A, B) :-
    parent(A, C),
    parent(C, B).

grandfather(A, B) :-
    grandparent(A, B),
    male(A).

grandmother(A, B) :-
    grandparent(A, B),
    female(A).

grandchild(A, B) :-
    grandparent(B, A).

greatgrandparent(A, B) :-
    parent(P, B),
    grandparent(A, P).

greatgrandchild(A, B) :-
    greatgrandparent(B, A).

granddaughter(A, B) :-
    grandchild(A, B),
    female(A).

grandson(A, B) :-
    grandchild(A, B),
    male(A).

uncle(A, B) :-
    brother(A, C),
    child(B, C).

aunt(A, B) :-
    sister(A, C),
    child(B, C).

cousin(A, B) :-
    grandparent(C, A),
    grandparent(C, B),
    \+sibling(A, B),
    A \= B.

nephew(A, B) :-
    aunt(B, A),
    male(A);
    uncle(B, A),
    male(A).

niece(A, B) :-
    aunt(B, A),
    female(A);
    uncle(B, A),
    female(A).

secondcousin(A, B) :-
    greatgrandparent(C, A),
    greatgrandparent(C, B),
    \+sibling(A, B),
    \+cousin(A, B),
    A \= B.
