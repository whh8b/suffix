#ifndef SUFFIX_H
#define SUFFIX_H

#include <unistd.h>
#include <assert.h>
#include <list>
#include <iostream>
#include <sstream>

class Locus
{
	public:
		Locus() :
			m_start(0),
			m_stop(0),
			m_descendents(-1),
			m_contracted_locus(NULL) {}

		int &Start() { return m_start; }
		int &Stop() { return m_stop; }
		int &Descendents() { return m_descendents; }
		Locus *&ContractedLocus() { return m_contracted_locus; }

		std::list<Locus*>::iterator ChildrenBegin() { return m_children.begin(); }
		std::list<Locus*>::iterator ChildrenEnd() { return m_children.end(); }
		void AddChild(Locus *new_child) { m_children.push_back(new_child); }
		void RemoveChild(Locus *child) { m_children.remove(child); }

		void AddTerminus(int index) { m_match_terminii.push_back(index); }
		std::list<int> Terminii() { return m_match_terminii; }
		void Terminii(const std::list<int> &new_terminii) {
			m_match_terminii=new_terminii;
		}

		friend std::ostream &operator<<(std::ostream &os, Locus &locus);

	private:
		int m_start, m_stop;
		int m_descendents;
		std::list<Locus*> m_children;
		Locus *m_contracted_locus;
		std::list<int> m_match_terminii;
};

template <template <typename...> class Container, typename Element>
class SuffixTreeBase
{
	public:
		SuffixTreeBase(bool verbose = true) : m_root(NULL), m_verbose(verbose) {}
		void Build(Container<Element> obj);
	
		friend std::ostream &operator<<(std::ostream &os, SuffixTreeBase<Container, Element> &st)
		{
			//os << st.entire << ":" << std::endl;
			st.DoPrint(os, st.m_root);
			return os;
		}
		void PrintSubstrings(std::ostream &, int occurence_filter = 0);

		std::list<Locus*>::const_iterator LociiBegin() const { return m_locus_list.begin(); }
		std::list<Locus*>::const_iterator LociiEnd() const { return m_locus_list.end(); }

	protected:
		void AddSuffix(unsigned int suffi);
		void DoInsert(unsigned int offset, Locus *locus);
		virtual bool not_equal(Element a, Element b)=0;
		virtual std::string StringifyElement(Element a)=0;
		void DoPrint(std::ostream &os, Locus *locus, int ws=0);
		int DoPrintSubstrings(std::ostream &os, Locus *locus, Container<Element> base, int occurence_filter);
		void BuildLocusList(Locus *locus);

		Locus *m_root;
		std::list<Locus*> m_locus_list;
		Container<Element> entire;
		bool m_verbose;
};

template <template <typename...> class Container, typename Element>
class SuffixTree : public SuffixTreeBase<Container, Element>
{
	public:
		SuffixTree(bool verbose): SuffixTreeBase<Container, Element>(verbose) {};
	protected:
		bool not_equal(Element a, Element b);
		std::string StringifyElement(Element a);
};

template <template <typename...> class Container, typename Element>
class SuffixTree<Container, Element*> : public SuffixTreeBase<Container, Element*>
{
	public:
		SuffixTree(bool verbose): SuffixTreeBase<Container, Element*>(verbose) { std::cout << "Constructing specialized ST" << std::endl; };
	protected:
		bool not_equal(Element *a, Element *b);
		std::string StringifyElement(Element *a);
};

#include "suffix.tcc"
#endif
