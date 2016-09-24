#ifndef PICTUS_ACTIONMAP_H
#define PICTUS_ACTIONMAP_H

#include <functional>
#include <map>

namespace App {
	template <typename _key, typename _param>
	class ActionMapParam {
	public:
		typedef typename std::function<void(_param)> Function_Type;
		void AddAction(_key id, Function_Type f) {
			m_map[id] = f;
		}

		bool Execute(_key id, _param e) {
			auto i = m_map.find(id);
			if(i == m_map.end())
				return false;

			(i->second)(e);
			return true;
		}

	private:
		typedef std::map<_key, Function_Type> FunctionMap;
		FunctionMap m_map;
	};


	template <typename _key>
	class ActionMapNoParam {
	public:
		typedef std::function<void()> Function_Type;
		void AddAction(_key id, Function_Type f) {
			m_map[id] = f;
		}

		bool Execute(_key id) {
			auto i = m_map.find(id);
			if(i == m_map.end())
				return false;

			(i->second)();
			return true;
		}

	private:
		typedef std::map<_key, Function_Type> FunctionMap;
		FunctionMap m_map;
	};
}

#endif
