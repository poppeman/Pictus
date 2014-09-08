#ifndef ACTIONMAP_H
#define ACTIONMAP_H

namespace App {
	template <typename _key, typename _param>
	class ActionMapParam {
	public:
		typedef boost::function<void(_param)> Function_Type;
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
		typedef boost::function<void()> Function_Type;
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
