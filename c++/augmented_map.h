#pragma once

using namespace std;

// *******************************************
//   AUG MAPS
// *******************************************

template <class _Entry, class Join_Tree>
struct aug_map_ : private map_<_Entry, Join_Tree> {
public:
  using Map = map_<_Entry, Join_Tree>;
  using Entry = typename Map::Entry;
  using Tree = augmented_ops<typename Map::Tree>;
  using node = typename Tree::node;
  using E = typename Entry::entry_t;
  using K = typename Entry::key_t;
  using V = typename Entry::val_t;
  using A = typename Entry::aug_t;
  using M = aug_map_;
  using GC = typename Map::GC;
  using maybe_V = maybe<V>;
  using maybe_E = maybe<E>;

  template<class F>
  static M aug_filter(M m, const F& f) {
    return M(Tree::aug_filter(m.get_root(), f)); }

  // extract the augmented values
  A aug_val() { return Tree::aug_val(Map::root); }   

  A aug_left (const K& key) {
    typename Tree::aug_sum_t a;
    Tree::aug_sum_left(Map::root, key, a);
    return a.result;}

  A aug_right(const K& key) {
    typename Tree::aug_sum_t a;
    Tree::aug_sum_right(Map::root, key, a);
    return a.result;}
  
  A aug_range(const K& key_left, const K& key_right) {
    typename Tree::aug_sum_t a;
    Tree::aug_sum_range(Map::root, key_left, key_right, a);
    return a.result;}

  // just side effecting
  template <class restricted_sum>
  void range_sum(const K& key_left, 
		 const K& key_right,
		 restricted_sum& rs) {
    Tree::aug_sum_range(Map::root, key_left, key_right, rs);
  }

  template <class Func>
  maybe_E aug_select(Func f) {
    return Map::node_to_entry(Tree::aug_select(Map::root, f));};

  static M insert_lazy(M m, const E& p) {
    auto replace = [] (const V& a, const V& b) {return b;};
    return M(Tree::insert_lazy(m.get_root(), p, replace)); }

  // for coercing a map to an aug_map
  aug_map_(Map&& m) {if (this != &m) {this->root = m.root; m.root = NULL;} }
  aug_map_() : Map() { }
  // install Map's constructors
  using Map::Map;
  // aug_map_(const M& m) : Map(m) {}
  // aug_map_(M&& m) : Map(m) {}
  // aug_map_(const E& e) : Map(e) {}
  // aug_map_(E* s, E* e, bool seq_inplace = false) {Map(s,e,seq_inplace);}
  // template<class Bin_Op>
  // aug_map_(E* s, E* e, Bin_Op f) {Map(s,e,f);}
  // aug_map_(sequence<E> S, bool seq_inplace = false) {Map(S,seq_inplace);}
  // template<class Seq, class Bin_Op>
  // aug_map_(Seq S, Bin_Op f, bool seq_inplace = false) {Map(S,f,seq_inplace);}
  // aug_map_(const M& m) : Map(m) {}
  // aug_map_(M&& m) : Map(std::move(m)) {}
  // M& operator = (const M& m) {Map::operator=(m);}
  // M& operator = (M&& m) {Map::operator=(std::move(m));}
  // ~aug_map_() {Map::clear(); }

  template <class Func>
  static M insert(M m, const E& p, const Func& f) {
    return Map::insert(std::move(m),p,f);}
  static M insert(M m, const E& p) {//cout << "ins a " << endl;
    return Map::insert(std::move(m),p);}
  static M remove(M m, const K& k) {return Map::remove(std::move(m), k);}
  template<class F>
  static M filter(M m, const F& f) {return Map::filter(std::move(m), f);}
  static M multi_insert(M m, sequence<E> SS, bool seq_inplace = false) {
    return Map::multi_insert(std::move(m), SS, seq_inplace);}
  template<class Bin_Op>
  static M multi_insert_combine(M m, sequence<E> S, Bin_Op f, 
				bool seq_inplace = false) {
    return Map::multi_insert_combine(std::move(m), S, f, seq_inplace);}
  template<class Val, class Reduce>
  static M multi_insert_reduce(M m, sequence<pair<K,Val>> S, Reduce g) {
    return Map::multi_insert_reduce(std::move(m), S, g); }
  template<class M1, class M2, class F>
  static M map_intersect(M1 a, M2 b, const F& op) {
    return Map::map_intersect(std::move(a), std::move(b), op);}
  static M map_intersect(M a, M b) {return Map::map_intersect(a, b);}
  template<class F>
  static M map_union(M a, M b, const F& op) {return Map::map_union(std::move(a), std::move(b), op);}
  static M map_union(M a, M b) {return Map::map_union(std::move(a), std::move(b));}
  static M map_difference(M a, M b) {return Map::map_difference(std::move(a), std::move(b));}
  static M range(M& a, K kl, K kr) {return Map::range(a,kl,kr);}
  template<class Ma, class F>
  static M map(Ma a, const F f) {return Map::map(a, f);}
  static void entries(M m, E* out) { Map::entries(std::move(m),out);}
  template <class outItter>
  static void keys(M m, outItter out) {Map::keys(std::move(m),out);}
  // maybe_V find(const K& k) {return Map::find(k);}
  // bool contains(const K& k) {return Map::contains(k);}
  // maybe_E next(const K& k) {return Map::next(k);}
  // maybe_E previous(const K& k) {return Map::previous(k);}
  // size_t rank(const K& k) { return Map::rank(k);}
  // maybe_E select(const size_t rank) {return Map::select(rank);}
  // void clear() {return Map::clear(); }
  bool operator == (const M& m) { return Map::operator==(m);}
  template<class Reduce>
  static typename Reduce::t map_reduce(M a, Reduce r) {
    return Map::map_reduce(a,r);}
  template<class Ma, class F>
  static M map_filter(Ma a, const F& f) {return Map::map_filter(a,f);}

public:
  using Map::map_reduce;
  using Map::size;
  using Map::is_empty;
  using Map::init;
  using Map::reserve;
  using Map::finish;
  using Map::clear;
  using Map::find;
  using Map::contains;
  using Map::next;
  using Map::previous;
  using Map::rank;
  using Map::select;
  using Map::root;
  using Map::get_root;
  using Map::insert;
};

// creates a key-value pair for the entry, and redefines from_entry
template <class entry>
struct aug_map_full_entry : entry {
  using val_t = typename entry::val_t;
  using key_t = typename entry::key_t;
  using aug_t = typename entry::aug_t;
  using entry_t = std::pair<key_t,val_t>;
  static inline key_t get_key(const entry_t& e) {return e.first;}
  static inline val_t get_val(const entry_t& e) {return e.second;}
  static inline void set_val(entry_t& e, const val_t& v) {e.second = v;}
  static inline aug_t from_entry(const entry_t& e) {
    return entry::from_entry(e.first, e.second);}
};

template <class _Entry, class Balance=weight_balanced_tree>
using aug_map =
  aug_map_<aug_map_full_entry<_Entry>,
  typename Balance::template
  balance<aug_node<typename Balance::data,aug_map_full_entry<_Entry>>>>;
