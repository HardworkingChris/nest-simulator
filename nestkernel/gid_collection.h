/*
 *  gid_collection.h
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GID_COLLECTION_H
#define GID_COLLECTION_H

// C++ includes:
#include <ostream>
#include <stdexcept> // out_of_range
#include <vector>

// Includes from libnestuil:
#include "lockptr.h"

// Includes from nestkernel:
#include "exceptions.h"
#include "nest_types.h"

// Includes from sli:
#include "arraydatum.h"
#include "tokenarray.h"

namespace nest
{
class GIDCollection;
class GIDCollectionPrimitive;
class GIDCollectionComposite;
class GIDCollectionMetadata;

typedef lockPTR< GIDCollection > GIDCollectionPTR;
typedef lockPTR< GIDCollectionMetadata > GIDCollectionMetadataPTR;

/**
 * Class for Metadata attached to GIDCollection.
 *
 * NEST modules that want to add metadata to GIDCollections they
 * create need to implement their own concrete subclass.
 */
class GIDCollectionMetadata
{
public:
  GIDCollectionMetadata()
  {
  }
  virtual ~GIDCollectionMetadata()
  {
  }
};

class GIDPair
{
public:
  index gid;
  index model_id;
};

/**
 * Iterator for GIDCollections.
 *
 * This iterator can iterate over primitive and composite GIDCollections.
 * Behavior is determined by the constructor used to create the iterator.
 */
class gc_const_iterator
{
  friend class GIDCollectionPrimitive;
  friend class GIDCollectionComposite;

private:
  GIDCollectionPTR coll_ptr_; //!< holds pointer reference in safe iterators
  size_t element_idx_;        //!< index into (current) primitive gid collection
  size_t part_idx_; //!< index into parts vector of composite collection

  /**
   * Pointer to primitive collection to iterate over.
   * Zero if iterator is for composite collection.
   */
  GIDCollectionPrimitive const* const primitive_collection_;

  /**
   * Pointer to composite collection to iterate over.
   * Zero if iterator is for primitive collection.
   */
  GIDCollectionComposite const* const composite_collection_;

  /**
   * Create iterator for GIDCollectionPrimitive.
   * @param collection  Collection to iterate over
   * @param offset  Index of collection element iterator points to
   */
  explicit gc_const_iterator( const GIDCollectionPrimitive& collection,
    size_t offset );

  /**
   * Create iterator for GIDCollectionComposite.
   * @param collection  Collection to iterate over
   * @param part    Index of part of collection iterator points to
   * @param offset  Index of element in part part that iterator points to
   */
  explicit gc_const_iterator( const GIDCollectionComposite& collection,
    size_t part,
    size_t offset );

  /**
   * Create safe iterator for GIDCollectionPrimitive.
   * @param collection_ptr lockptr to collection to keep collection alive
   * @param collection  Collection to iterate over
   * @param offset  Index of collection element iterator points to
   */
  explicit gc_const_iterator( GIDCollectionPTR collection_ptr,
    const GIDCollectionPrimitive& collection,
    size_t offset );

  /**
   * Create safe iterator for GIDCollectionComposite.
   * @param collection_ptr lockptr to collection to keep collection alive
   * @param collection  Collection to iterate over
   * @param part    Index of part of collection iterator points to
   * @param offset  Index of element in part part that iterator points to
   */
  explicit gc_const_iterator( GIDCollectionPTR collection_ptr,
    const GIDCollectionComposite& collection,
    size_t part,
    size_t offset );

public:
  gc_const_iterator( const gc_const_iterator& );

  GIDPair operator*() const;
  bool operator!=( const gc_const_iterator& rhs ) const;

  gc_const_iterator& operator++();

  void print_me( std::ostream& ) const;
};

class GIDCollection
{
  friend class gc_const_iterator;

public:
  typedef gc_const_iterator const_iterator;

  virtual ~GIDCollection()
  {
    std::cerr << "Deleting GC: " << this << std::endl;
  }

  static GIDCollectionPTR create( IntVectorDatum gids );
  static GIDCollectionPTR create( TokenArray gids );

  virtual void print_me( std::ostream& ) const = 0;

  virtual index operator[]( size_t ) const = 0;
  virtual GIDCollectionPTR operator+( GIDCollectionPTR ) const = 0;
  virtual bool operator==( GIDCollectionPTR ) const = 0;
  virtual bool operator!=( GIDCollectionPTR ) const;

  virtual const_iterator begin() const = 0;
  virtual const_iterator begin( GIDCollectionPTR ) const = 0;
  virtual const_iterator end() const = 0;
  virtual const_iterator end( GIDCollectionPTR ) const = 0;

  virtual ArrayDatum to_array() const = 0;

  virtual size_t size() const = 0;

  virtual bool contains( index gid ) const = 0;
  virtual GIDCollectionPTR
  slice( size_t first, size_t last, size_t step ) const = 0;

  virtual void set_metadata( GIDCollectionMetadataPTR );

  virtual GIDCollectionMetadataPTR get_metadata() const = 0;

private:
  static GIDCollectionPTR create_( const std::vector< index >& );
};

class GIDCollectionPrimitive : public GIDCollection
{
  friend class gc_const_iterator;

  using GIDCollection::begin;
  using GIDCollection::end;

private:
  index first_;
  index last_;
  index model_id_;
  GIDCollectionMetadataPTR metadata_;

public:
  typedef gc_const_iterator const_iterator;

  GIDCollectionPrimitive( index first,
    index last,
    index model_id,
    GIDCollectionMetadataPTR );
  GIDCollectionPrimitive( index first, index last, index model_id );
  GIDCollectionPrimitive( index first, index last );
  GIDCollectionPrimitive( const GIDCollectionPrimitive& );

  void print_me( std::ostream& ) const;

  index operator[]( const size_t ) const;
  GIDCollectionPTR operator+( GIDCollectionPTR rhs ) const;
  bool operator==( const GIDCollectionPTR rhs ) const;
  bool operator==( const GIDCollectionPrimitive& rhs ) const;

  const_iterator begin() const;
  const_iterator begin( GIDCollectionPTR ) const;
  const_iterator end() const;
  const_iterator end( GIDCollectionPTR ) const;

  ArrayDatum to_array() const;

  size_t size() const;

  bool contains( index gid ) const;
  GIDCollectionPTR slice( size_t first, size_t last, size_t step = 1 ) const;

  void set_metadata( GIDCollectionMetadataPTR );

  GIDCollectionMetadataPTR get_metadata() const;
};

GIDCollectionPTR operator+( GIDCollectionPTR lhs, GIDCollectionPTR rhs );

class GIDCollectionComposite : public GIDCollection
{
  friend class gc_const_iterator;

  using GIDCollection::begin;
  using GIDCollection::end;

private:
  std::vector< GIDCollectionPrimitive > parts_;
  size_t size_; // total number of GIDs
public:
  GIDCollectionComposite( const GIDCollectionPrimitive& prim,
    size_t start,
    size_t stop,
    size_t step );
  GIDCollectionComposite( const GIDCollectionComposite& );
  GIDCollectionComposite( const std::vector< GIDCollectionPrimitive >& );

  void print_me( std::ostream& ) const;

  index operator[]( const size_t ) const;
  GIDCollectionPTR operator+( GIDCollectionPTR rhs ) const;
  GIDCollectionPTR operator+( const GIDCollectionPrimitive& rhs ) const;
  bool operator==( const GIDCollectionPTR rhs ) const;

  const_iterator begin() const;
  const_iterator begin( GIDCollectionPTR ) const;
  const_iterator end() const;
  const_iterator end( GIDCollectionPTR ) const;

  ArrayDatum to_array() const;

  size_t size() const;

  bool contains( index gid ) const;
  GIDCollectionPTR slice( size_t first, size_t last, size_t step = 1 ) const;

  GIDCollectionMetadataPTR get_metadata() const;
};

inline bool GIDCollection::operator!=( GIDCollectionPTR rhs ) const
{
  return not( *this == rhs );
}

inline void GIDCollection::set_metadata( GIDCollectionMetadataPTR )
{
  throw KernelException( "Cannot set Metadata on this type of GIDCollection." );
}

inline GIDPair gc_const_iterator::operator*() const
{
  GIDPair gp;
  if ( primitive_collection_ )
  {
    gp.gid = primitive_collection_->first_ + element_idx_;
    if ( gp.gid > primitive_collection_->last_ )
    {
      throw KernelException( "Invalid GIDCollection iterator " );
    }
    gp.model_id = primitive_collection_->model_id_;
  }
  else
  {
    if ( part_idx_ >= composite_collection_->parts_.size()
      or element_idx_ >= composite_collection_->parts_[ part_idx_ ].size() )
    {
      throw KernelException( "Invalid GIDCollection iterator " );
    }
    gp.gid = composite_collection_->parts_[ part_idx_ ][ element_idx_ ];
    gp.model_id = composite_collection_->parts_[ part_idx_ ].model_id_;
  }
  return gp;
}

inline gc_const_iterator& gc_const_iterator::operator++()
{
  if ( primitive_collection_ )
  {
    ++element_idx_;
    return *this;
  }
  else
  {
    ++element_idx_;
    if ( element_idx_ >= composite_collection_->parts_[ part_idx_ ].size() )
    {
      ++part_idx_;
      element_idx_ = 0;
    }
    return *this;
  }
}

inline bool gc_const_iterator::operator!=( const gc_const_iterator& rhs ) const
{
  return not( part_idx_ == rhs.part_idx_ and element_idx_ == rhs.element_idx_ );
}

inline index GIDCollectionPrimitive::operator[]( const size_t idx ) const
{
  // throw exception if outside of GIDCollection
  if ( first_ + idx > last_ )
    throw std::out_of_range( "pos points outside of the GIDCollection" );
  return first_ + idx;
}

inline bool GIDCollectionPrimitive::operator==( GIDCollectionPTR rhs ) const
{
  GIDCollectionPrimitive const* const rhs_ptr =
    dynamic_cast< GIDCollectionPrimitive const* >( rhs.get() );
  rhs.unlock();

  return first_ == rhs_ptr->first_ and last_ == rhs_ptr->last_
    and model_id_ == rhs_ptr->model_id_ and metadata_ == rhs_ptr->metadata_;
}

inline bool GIDCollectionPrimitive::operator==(
  const GIDCollectionPrimitive& rhs ) const
{
  return first_ == rhs.first_ and last_ == rhs.last_
    and model_id_ == rhs.model_id_ and metadata_ == rhs.metadata_;
}

inline GIDCollectionPrimitive::const_iterator
GIDCollectionPrimitive::begin() const
{
  return const_iterator( *this, 0 );
}

inline GIDCollectionPrimitive::const_iterator
GIDCollectionPrimitive::begin( GIDCollectionPTR cp ) const
{
  return const_iterator( cp, *this, 0 );
}

inline GIDCollectionPrimitive::const_iterator
GIDCollectionPrimitive::end() const
{
  return const_iterator( *this, size() );
}

inline GIDCollectionPrimitive::const_iterator
GIDCollectionPrimitive::end( GIDCollectionPTR cp ) const
{
  return const_iterator( cp, *this, size() );
}

inline size_t
GIDCollectionPrimitive::size() const
{
  return last_ - first_ + 1;
}

inline bool
GIDCollectionPrimitive::contains( index gid ) const
{
  return first_ <= gid and gid <= last_;
}

inline void
GIDCollectionPrimitive::set_metadata( GIDCollectionMetadataPTR meta )
{
  metadata_ = meta;
}

inline GIDCollectionMetadataPTR
GIDCollectionPrimitive::get_metadata() const
{
  return metadata_;
}


// composite []
inline index GIDCollectionComposite::operator[]( const size_t i ) const
{
  long tot_prev_gids = 0;
  for (
    std::vector< GIDCollectionPrimitive >::const_iterator gc = parts_.begin();
    gc != parts_.end();
    ++gc ) // iterate over GIDCollections
  {
    if ( tot_prev_gids + ( *gc ).size() > i ) // is i in current GIDCollection?
    {
      long local_i = i - tot_prev_gids; // get local i
      return ( *gc )[ local_i ];
    }
    else // if is not in current GIDCollection
    {
      tot_prev_gids += ( *gc ).size();
    }
  }
  // throw exception if outside of GIDCollection
  throw std::out_of_range( "pos points outside of the GIDCollection" );
}


// composite ==
inline bool GIDCollectionComposite::operator==( GIDCollectionPTR rhs ) const
{
  GIDCollectionComposite const* const rhs_ptr =
    dynamic_cast< GIDCollectionComposite const* >( rhs.get() );
  rhs.unlock();

  if ( size_ != rhs_ptr->size() || parts_.size() != rhs_ptr->parts_.size() )
  {
    return false;
  }
  std::vector< GIDCollectionPrimitive >::const_iterator rhs_gc =
    rhs_ptr->parts_.begin();
  for ( std::vector< GIDCollectionPrimitive >::const_iterator
          lhs_gc = parts_.begin();
        lhs_gc != parts_.end();
        ++lhs_gc, ++rhs_gc ) // iterate over GIDCollections
  {
    if ( not( ( *lhs_gc ) == ( *rhs_gc ) ) )
    {
      return false;
    }
  }
  return true;
}

inline GIDCollectionComposite::const_iterator
GIDCollectionComposite::begin() const
{
  return const_iterator( *this, 0, 0 );
}

inline GIDCollectionComposite::const_iterator
GIDCollectionComposite::begin( GIDCollectionPTR cp ) const
{
  return const_iterator( cp, *this, 0, 0 );
}

inline GIDCollectionComposite::const_iterator
GIDCollectionComposite::end() const
{
  return const_iterator( *this, parts_.size(), 0 );
}

inline GIDCollectionComposite::const_iterator
GIDCollectionComposite::end( GIDCollectionPTR cp ) const
{
  return const_iterator( cp, *this, parts_.size(), 0 );
}

inline size_t
GIDCollectionComposite::size() const
{
  return size_;
}

inline bool
GIDCollectionComposite::contains( index gid ) const
{
  for (
    std::vector< GIDCollectionPrimitive >::const_iterator gc = parts_.begin();
    gc != parts_.end();
    ++gc ) // iterate over GIDCollections
  {
    if ( ( *gc ).contains( gid ) )
    {
      return true;
    }
  }
  return false;
}

inline GIDCollectionMetadataPTR
GIDCollectionComposite::get_metadata() const
{
  return parts_[ 0 ].get_metadata();
}

} // namespace nest

#endif /* #ifndef GID_COLLECTION_H */
