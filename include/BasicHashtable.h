/*
 * gleem -- OpenGL Extremely Easy-To-Use Manipulators.
 * Copyright (C) 1998 Kenneth B. Russell (kbrussel@media.mit.edu)
 * See the file LICENSE.txt in the doc/ directory for licensing terms.
 */

/*
 * A simple hash table wrapper for SGI's STL version.
 * Kenneth B. Russell (kbrussel@media.mit.edu); public domain.
 */

#ifndef _BASIC_HASHTABLE_H
#define _BASIC_HASHTABLE_H

#include "../src/stl/hashtable.h"
#include "../src/stl/pair.h"

template <class Value, class Key, class HashFcn>
  class BasicHashtable
{
 private:
  typedef pair<Key, Value> KeyValuePair;
  typedef size_t HashFunc(const Key &);
  typedef Key    ExtractKeyFunc(const KeyValuePair &);
  typedef int    EqualFunc(const Key &, const Key &);
  
  static int equals(const Key &arg1, const Key &arg2) {
    return (arg1 == arg2);
  }
  static Key extractKey(const KeyValuePair &arg) {
    return arg.first;
  }

  typedef hashtable<KeyValuePair, Key, HashFcn,
    ExtractKeyFunc *, EqualFunc *> InternalHashtable;
  InternalHashtable table;

 public:
  class iterator
  {
  public:
    typedef Value &ValueReference;
    typedef Value *ValuePointer;
    typedef Key &KeyReference;

    ValueReference operator *() const { return (*internalIter).second; }
#if 0
#ifndef __SGI_STL_NO_ARROW_OPERATOR
    ValuePointer operator->() const { return &(operator*()); }
#endif /* __SGI_STL_NO_ARROW_OPERATOR */
#endif
    
    KeyReference getKey() const { return (*internalIter).first; }

    // prefix
    iterator& operator++() { ++internalIter; return *this; }
    // postfix
    iterator operator++(int) { iterator it = *this; ++internalIter; return it; }
    bool operator==(const iterator& it) const { return internalIter == it.internalIter; }
    bool operator!=(const iterator& it) const { return internalIter != it.internalIter; }

    friend class BasicHashtable<Value, Key, HashFcn>;
  private:
  iterator(InternalHashtable::iterator iter) :
    internalIter(iter) {}

    InternalHashtable::iterator internalIter;
  };

  class const_iterator
  {
  public:
    typedef const Value &ValueReference;
    typedef const Value *ValuePointer;
    typedef const Key &KeyReference;

    ValueReference operator *() const { return (*internalIter).second; }
#if 0
#ifndef __SGI_STL_NO_ARROW_OPERATOR
    ValuePointer operator->() const { return &(operator*()); }
#endif /* __SGI_STL_NO_ARROW_OPERATOR */
#endif
    
    KeyReference getKey() const { return (*internalIter).first; }

    // prefix
    const_iterator& operator++() { ++internalIter; return *this; }
    // postfix
    const_iterator operator++(int) { const_iterator it = *this; ++internalIter; return it; }
    bool operator==(const const_iterator& it) const { return internalIter == it.internalIter; }
    bool operator!=(const const_iterator& it) const { return internalIter != it.internalIter; }

    friend class BasicHashtable<Value, Key, HashFcn>;
  private:
  const_iterator(InternalHashtable::const_iterator iter) :
    internalIter(iter) {}

    InternalHashtable::const_iterator internalIter;
  };

 BasicHashtable(size_t n,
		const HashFcn &hf) :
  table(n, hf, &BasicHashtable::equals, &BasicHashtable::extractKey) {}

  size_t size() const { return table.size(); }
  size_t max_size() const { return table.max_size(); }
  bool empty() const { return table.empty(); }
  
  void swap(BasicHashtable& ht) { table.swap(ht.table); }
  
  iterator begin()
  {
    return iterator(table.begin());
  }

  iterator end()
  {
    return iterator(table.end());
  }

  const_iterator begin() const
  {
    return const_iterator(table.begin());
  }

  const_iterator end() const
  {
    return const_iterator(table.end());
  }

  friend bool
    operator== __STL_NULL_TMPL_ARGS (const BasicHashtable&, const BasicHashtable&);

  size_t bucket_count() const { return table.bucket_count(); }
  
  size_t max_bucket_count() const { return table.max_bucket_count(); }

  size_t elems_in_bucket(size_t bucket) const
  {
    return table.elems_in_bucket(bucket);
  }
  
  pair<iterator, bool> insert_unique(const Key &key,
				     const Value &val)
    {
      pair<InternalHashtable::iterator, bool> result =
	table.insert_unique(KeyValuePair(key, val));
      return pair<iterator, bool>(iterator(result.first), result.second);
    }  
  
  iterator insert_equal(const Key &key,
			const Value &val)
  {
    return iterator(table.insert_equal(KeyValuePair(key, val)));
  }
  
  pair<iterator, bool> insert_unique_noresize(const Key &key,
					      const Value &val)
    {
      pair<InternalHashtable::iterator, bool> result =
	table.insert_unique_noresize(KeyValuePair(key, val));
      return pair<iterator, bool>(iterator(result.first), result.second);
    }

  iterator insert_equal_noresize(const Key &key,
				 const Value &val)
  {
    return iterator(table.insert_equal_noresize(KeyValuePair(key, val)));
  }

  // FIXME: insertion with iterators elided because it is not clear
  // how useful they are (would require pointers to pair<Key, Value>
  // objects)
  
  Value &find_or_insert(const Key &key,
			const Value &value)
    {
      return table.find_or_insert(KeyValuePair(key, value)).second;
    }

  iterator find(const Key &key)
  {
    return iterator(table.find(key));
  }
  
  const_iterator find(const Key &key) const
  {
    return const_iterator(table.find(key));
  }
  
  size_t count(const Key &key) const
  {
    return table.count(key);
  }

  pair<iterator, iterator> equal_range(const Key &key)
    {
      pair<InternalHashtable::iterator, InternalHashtable::iterator> result =
	table.equal_range(key);
      return pair<iterator, iterator>(result.first, result.second);
    }

  pair<const_iterator, const_iterator> equal_range(const Key &key) const
    {
      pair<InternalHashtable::const_iterator, InternalHashtable::const_iterator> result =
	table.equal_range(key);
      return pair<const_iterator, const_iterator>(result.first, result.second);
    }

  size_t erase(const Key &key)
  {
    return table.erase(key);
  }

  void erase(const iterator &it)
  {
    table.erase(it.internalIter);
  }

  void erase(iterator first, iterator last)
  {
    table.erase(first.internalIter, last.internalIter);
  }

  void erase(const const_iterator &it)
  {
    table.erase(it.internalIter);
  }

  void erase(const_iterator first, const_iterator last)
  {
    table.erase(first.internalIter, last.internalIter);
  }

  void resize(size_t num_elements_hint)
  {
    table.resize(num_elements_hint);
  }

  void clear()
  {
    table.clear();
  }

};

template<class Value, class Key, class HashFcn>
bool operator==(const BasicHashtable<Value, Key, HashFcn> &ht1,
		const BasicHashtable<Value, Key, HashFcn> &ht2)
{
  return (ht1.table == ht2.table);
}

#endif  // #defined _BASIC_HASHTABLE_H
