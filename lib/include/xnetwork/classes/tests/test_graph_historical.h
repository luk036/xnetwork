// !file C++17
/** Original XNetwork graph tests */
from nose.tools import *
import xnetwork
#include <xnetwork.hpp>using namespace xn;

from historical_tests import HistoricalTests


class TestGraphHistorical(HistoricalTests) {

    auto setUp( ) {
        HistoricalTests.setUp( );
        this->G = xn::Graph
