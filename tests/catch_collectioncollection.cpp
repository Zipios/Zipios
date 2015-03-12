/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000-2015  Thomas Sondergaard

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

/** \file
 *
 * Zipios++ unit tests for the CollectionCollection class.
 */

#include "catch_tests.hpp"

#include "zipios++/collectioncollection.hpp"
#include "zipios++/zipiosexceptions.hpp"

//#include "src/dostime.h"
//
//#include <fstream>
//#include <memory>
//#include <vector>
//
//#include <unistd.h>
//#include <string.h>




SCENARIO("CollectionCollection with various tests", "[DirectoryCollection] [FileCollection]")
{
    GIVEN("an empty collection collection")
    {
        zipios::CollectionCollection cc;

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            // an empty CollectionCollection is valid
            REQUIRE(cc.isValid());
            REQUIRE(cc.entries().empty());
            REQUIRE_FALSE(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(cc.getName() == "-");   // default name is "-"
            REQUIRE(cc.size() == 0);
            cc.mustBeValid();

            zipios::CollectionCollection copy_constructor(cc);
            REQUIRE(copy_constructor.isValid());
            REQUIRE(copy_constructor.entries().empty());
            REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(copy_constructor.getName() == "-");   // copy name as is
            REQUIRE(copy_constructor.size() == 0);
            copy_constructor.mustBeValid();

            zipios::CollectionCollection copy_assignment;
            copy_assignment = cc;
            REQUIRE(copy_assignment.isValid());
            REQUIRE(copy_assignment.entries().empty());
            REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(copy_assignment.getName() == "-");   // copy name as is
            REQUIRE(copy_assignment.size() == 0);
            copy_assignment.mustBeValid();

            zipios::FileCollection::pointer_t clone(cc.clone());
            REQUIRE(clone->isValid());
            REQUIRE(clone->entries().empty());
            REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
            REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
            REQUIRE(clone->getName() == "-");   // copy name as is
            REQUIRE(clone->size() == 0);
            clone->mustBeValid();
        }

        WHEN("closing the directory")
        {
            cc.close();

            THEN("it is now invalid")
            {
                REQUIRE_FALSE(cc.isValid());
                REQUIRE_THROWS_AS(cc.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.getName() == "-", zipios::InvalidStateException);   // default name is "-"
                REQUIRE_THROWS_AS(cc.size() == 0, zipios::InvalidStateException);
                REQUIRE_THROWS_AS(cc.mustBeValid(), zipios::InvalidStateException);

                zipios::CollectionCollection copy_constructor(cc);
                REQUIRE_FALSE(copy_constructor.isValid());
                REQUIRE_THROWS_AS(copy_constructor.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getName() == "-", zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(copy_constructor.size() == 0, zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.mustBeValid(), zipios::InvalidStateException);

                zipios::CollectionCollection copy_assignment;
                copy_assignment = cc;
                REQUIRE_FALSE(copy_assignment.isValid());
                REQUIRE_THROWS_AS(copy_assignment.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getName() == "-", zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(copy_assignment.size() == 0, zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.mustBeValid(), zipios::InvalidStateException);

                zipios::FileCollection::pointer_t clone(cc.clone());
                REQUIRE_FALSE(clone->isValid());
                REQUIRE_THROWS_AS(clone->entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getName() == "-", zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(clone->size() == 0, zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->mustBeValid(), zipios::InvalidStateException);
            }
        }
    }
}


// vim: ts=4 sw=4 et
