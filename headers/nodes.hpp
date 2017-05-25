#ifndef HEADER_NODES_HPP_INCLUDED
#define HEADER_NODES_HPP_INCLUDED

#include "advanced_vector.hpp"

namespace __node_namespace {
	template< typename _T1, typename _T2 >
	class _DeramidNode {
	public:

		///////////////////////////////////////////////////////////////////////////////

		_DeramidNode* left;
		_DeramidNode* right;

		_T1 key;
		_T2	priority;

		///////////////////////////////////////////////////////////////////////////////

		_DeramidNode(_T1 key, _T2 priority) :
			key(key), priority(priority), left(NULL), right(NULL)
		{}
		_DeramidNode(_T1 key, _T2 priority, _DeramidNode* left, _DeramidNode* rigth) :
			key(key), priority(priority), left(left), right(right)
		{}
		_DeramidNode(_DeramidNode& other) :
			key(other.key), priority(other.priority), left(other.left), right(other.right)
		{}
		_DeramidNode(_DeramidNode&& other) :
			key(other.key), priority(other.priority), left(other.left), right(other.right)
		{}

		~_DeramidNode() {}

		///////////////////////////////////////////////////////////////////////////////

	    static constexpr inline _DeramidNode* Merge(_DeramidNode< _T1, _T2 >* leftSubtree,
									_DeramidNode< _T1, _T2 >* rightSubtree) {
	        if (leftSubtree == NULL)
				return rightSubtree;
			if (rightSubtree == NULL)
				return leftSubtree;
			if (leftSubtree->priority > rightSubtree->priority) {
	            leftSubtree->right = Merge(leftSubtree->right, rightSubtree);
	            return leftSubtree;
			}
			else {
				rightSubtree->left = Merge(leftSubtree, rightSubtree->left);
				return rightSubtree;
			}
	    }

	    static constexpr inline void Split(_DeramidNode< _T1, _T2 >* tree, _T1 splitKey,
											_DeramidNode< _T1, _T2 >** leftSubtree,
											_DeramidNode< _T1, _T2 >** rightSubtree) {
			if (tree->key <= splitKey) {
				if (tree->right == NULL) {
					*rightSubtree = NULL;
					*leftSubtree = tree;
					return;
				}
				else {
					_DeramidNode< _T1, _T2 >* tmp = NULL;
	                Split(tree->right, splitKey, &tmp, rightSubtree);
	                *leftSubtree = tree;
	                (*leftSubtree)->right = tmp;
	                return;
				}
			}
			else {
	            if (tree->left == NULL) {
					*leftSubtree = NULL;
					*rightSubtree = tree;
					return;
	            }
	            else {
					_DeramidNode< _T1, _T2 >* tmp = NULL;
	                Split(tree->left, splitKey, leftSubtree, &tmp);
	                *rightSubtree = tree;
	                (*rightSubtree)->left = tmp;
	                return;
				}
			}
		}

		static constexpr inline _DeramidNode* Insert(_DeramidNode< _T1, _T2 >* tree, _T1 key)
		{
			if (tree == NULL)
				return new _DeramidNode< _T1, _T2 >(key, rand());

			_DeramidNode< _T1, _T2 >* l = NULL;
			_DeramidNode< _T1, _T2 >* r = NULL;
			Split(tree, key, &l, &r);
			_DeramidNode< _T1, _T2 >* m = new _DeramidNode< _T1, _T2 >(key, rand());
			return Merge(Merge(l, m), r);
		}

		static constexpr inline _DeramidNode* Remove(_DeramidNode< _T1, _T2 >* tree, _T1 key)
		{
			_DeramidNode< _T1, _T2 >* l = NULL;
			_DeramidNode< _T1, _T2 >* m = NULL;
			_DeramidNode< _T1, _T2 >* r = NULL;
			Split(tree, key - 1, &l, &r);
			Split(r, key, &m, &r);
			delete m;
			return Merge(l, r);
		}

		///////////////////////////////////////////////////////////////////////////////
		static void Destroy(_DeramidNode< _T1, _T2 >* tree)
		{
			if (tree == NULL)
				return;
			Destroy(tree->left);
			Destroy(tree->right);
			delete tree;
		}


	    void Dump(int depth)
	    {
	    	if (this->left != NULL)
				this->left->Dump(depth + 1);
	        for (int i = 0; i < depth; i++)
				std::cout << "   |";
			std::cout << this->key << " : " << this->priority << std::endl;
			if (this->right != NULL)
				this->right->Dump(depth + 1);
	    }

	};


	template< typename _T >
	class _ImplicitNode {

	public:
		///////////////////////////////////////////////////////////////////////////////

		_T cost;
	    unsigned long long int priority;
	    _ImplicitNode* left;
	    _ImplicitNode* right;
		unsigned sz;

	    ///////////////////////////////////////////////////////////////////////////////

		_ImplicitNode(_T cost, unsigned long long int priority) :
			cost(cost), priority(priority), left(NULL), right(NULL), sz(1)
		{}
		_ImplicitNode(_T cost, unsigned long long int priority,
							_ImplicitNode* left, _ImplicitNode* right) :
			cost(cost), priority(priority), left(left), right(right), sz(1)
		{}
	    _ImplicitNode(_ImplicitNode& other) :
			cost(other.cost), priority(other.priority),
			left(other.left), right(other.right), sz(other.sz)
		{}
		_ImplicitNode(_ImplicitNode&& other) :
			cost(other.cost), priority(other.priority),
			left(other.left), right(other.right), sz(other.sz)
		{}

		///////////////////////////////////////////////////////////////////////////////

		static constexpr inline unsigned getSize(_ImplicitNode< _T >* tree) {
			return tree == NULL ? 0 : tree->sz;
		}

		static constexpr inline unsigned recalc(_ImplicitNode< _T >* tree) {
			tree->sz = getSize(tree->left) + getSize(tree->right) + 1;
			return tree->sz;
		}

		///////////////////////////////////////////////////////////////////////////////

		static constexpr inline _ImplicitNode* Merge(
								_ImplicitNode< _T >* leftSubtree,
								_ImplicitNode< _T >* rightSubtree) {
	        if (leftSubtree == NULL)
				return rightSubtree;
			if (rightSubtree == NULL)
				return leftSubtree;
			if (leftSubtree->priority > rightSubtree->priority) {
	            leftSubtree->right = Merge(leftSubtree->right, rightSubtree);
	            recalc(leftSubtree);
	            return leftSubtree;
			}
			else {
				rightSubtree->left = Merge(leftSubtree, rightSubtree->left);
				recalc(rightSubtree);
				return rightSubtree;
			}
	    }

	    static constexpr inline void Split(_ImplicitNode< _T >* tree, unsigned position,
											_ImplicitNode< _T >** leftSubtree,
											_ImplicitNode< _T >** rightSubtree) {

	        int curIndex = getSize(tree->left) + 1;

			if (curIndex <= ( int ) position) {
				if (tree->right == NULL) {
					*rightSubtree = NULL;
					*leftSubtree = tree;
					return;
				}
				else {
					_ImplicitNode< _T >* tmp = NULL;
	                Split(tree->right, position - curIndex, &tmp, rightSubtree);
	                *leftSubtree = tree;
	                (*leftSubtree)->right = tmp;
	                recalc(*leftSubtree);
	                return;
				}
			}
			else {
	            if (tree->left == NULL) {
					*leftSubtree = NULL;
					*rightSubtree = tree;
					return;
	            }
	            else {
					_ImplicitNode< _T >* tmp = NULL;
	                Split(tree->left, position, leftSubtree, &tmp);
	                *rightSubtree = tree;
	                (*rightSubtree)->left = tmp;
	                recalc(*rightSubtree);
	                return;
				}
			}
		}

		static constexpr inline _ImplicitNode* Insert(_ImplicitNode< _T >* tree,
					unsigned pos, _T cost, unsigned long long int priority)
		{
			if (tree == NULL)
				return new _ImplicitNode< _T >(cost, priority);

			_ImplicitNode< _T >* l = NULL;
			_ImplicitNode< _T >* r = NULL;
			Split(tree, pos, &l, &r);
			_ImplicitNode< _T >* m = new _ImplicitNode< _T >(cost, priority);
			return Merge(Merge(l, m), r);
		}

		static constexpr inline _ImplicitNode< _T >* Remove(_ImplicitNode< _T >* tree, unsigned pos)
		{
			_ImplicitNode< _T >* l = NULL;
			_ImplicitNode< _T >* m = NULL;
			_ImplicitNode< _T >* r = NULL;
			Split(tree, pos - 1, &l, &r);
			Split(r, 1, &m, &r);
			delete m;
			return Merge(l, r);
		}

		static constexpr inline _ImplicitNode< _T >* Copy(_ImplicitNode< _T >* tree)
		{
	        if (tree == NULL)
				return NULL;

			_ImplicitNode< _T >* result =
				new _ImplicitNode< _T >(tree->cost, tree->priority, Copy(tree->left), Copy(tree->right));
			result->sz = tree->sz;

			return result;
		}

		///////////////////////////////////////////////////////////////////////////////
		static void Destroy(_ImplicitNode< _T >* tree)
		{
			if (tree == NULL)
				return;
			Destroy(tree->left);
			Destroy(tree->right);
			delete tree;
		}


	    void Dump(int depth) {
	    	if (this->left != NULL)
				this->left->Dump(depth + 1);
	        for (int i = 0; i < depth; i++)
				std::cout << "   |";
			std::cout << this->sz << " : " << this->cost << " : " << this->priority << std::endl;
			if (this->right != NULL)
				this->right->Dump(depth + 1);
	    }
		///////////////////////////////////////////////////////////////////////////////


	};

}

#endif
