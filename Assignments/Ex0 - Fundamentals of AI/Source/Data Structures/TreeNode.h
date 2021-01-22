#include<stdint.h>
#include<memory>
#include<vector>
#include<deque>

// TreeNode class should go in the "ufl_cap4053::fundamentals" namespace!
namespace ufl_cap4053 { namespace fundamentals {
		

	template<typename T>
	class TreeNode {
		using DataFunction = auto (*)(const T) -> void;
	public:
		TreeNode<T>() {}
		TreeNode<T>(T element) : data(element) {}
		auto getData() const -> const T&;
		auto getChildCount() const->size_t;
		auto getChild(size_t index) const->TreeNode<T>*;
		auto addChild(TreeNode<T>* child) -> void;
		auto removeChild(size_t index)->void;
		auto breadthFirstTraverse(DataFunction functor) const -> void;
		auto preOrderTraverse(DataFunction functor) const -> void;
		auto postOrderTraverse(DataFunction functor) const -> void;
	private:
		std::vector<TreeNode<T>*> children;
		T data{};
	};

	template<typename T>
	auto TreeNode<T>::getData() const -> const T& {
		return data;
	}

	template<typename T>
	auto TreeNode<T>::getChildCount() const->size_t {
		return children.size();
	}

	template<typename T>
	auto TreeNode<T>::getChild(size_t index) const->TreeNode<T>* {
		return children.at(index);
	}

	template<typename T>
	auto TreeNode<T>::addChild(TreeNode<T>* child) -> void {
		children.push_back(child);
	}

	template<typename T>
	auto TreeNode<T>::removeChild(size_t index)->void {
		children.erase(children.begin() + index);
	}

	template<typename T>
	auto TreeNode<T>::breadthFirstTraverse(DataFunction functor) const -> void {
		functor(data);
		std::vector<TreeNode<T>*> toVisit(children.rbegin(), children.rend());
		while (!toVisit.empty()) {
			functor(toVisit.back()->data);
			toVisit.insert(
				toVisit.begin(), 
				toVisit.back()->children.rbegin(),
				toVisit.back()->children.rend()
			);
			toVisit.pop_back();
		}
	}

	template<typename T>
	auto TreeNode<T>::preOrderTraverse(DataFunction functor) const -> void {
		functor(data);
		for (const auto& child : children) {
			child->preOrderTraverse(functor);
		}
	}

	template<typename T>
	auto TreeNode<T>::postOrderTraverse(DataFunction functor) const -> void {
		for (const auto& child : children) {
			child->postOrderTraverse(functor);
		}
		functor(data);
	}
}
}  // namespace ufl_cap4053::fundamentals
