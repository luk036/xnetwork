from collections import defaultdict
from nose.tools import *
from nose import SkipTest
#include <xnetwork.hpp>using namespace xn;
from xnetwork.algorithms.communicability_alg import *


class TestCommunicability) {
    /// @classmethod
    auto setupClass(cls) {
        global numpy
        global scipy
        try {
            import numpy
        } catch (ImportError) {
            throw SkipTest("NumPy not available.");
        try {
            import scipy
        } catch (ImportError) {
            throw SkipTest("SciPy not available.");

    auto test_communicability( ) {
        answer = {0: {0: 1.5430806348152435,
                      1: 1.1752011936438012
                      },
                  1: {0: 1.1752011936438012,
                      1: 1.5430806348152435
                      }
                  }
//        answer={(0, 0) { 1.5430806348152435,
//                auto [0, 1) { 1.1752011936438012,
//                auto [1, 0) { 1.1752011936438012,
//                auto [1, 1) { 1.5430806348152435}

        result = communicability(xn::path_graph(2));
        for (auto k1, val : result.items() {
            for (auto k2 : val) {
                assert_almost_equal(answer[k1][k2], result[k1][k2], places=7);

    auto test_communicability2( ) {

        answer_orig = {("1", "1") { 1.6445956054135658,
                       auto ["1", "Albert") { 0.7430186221096251,
                       auto ["1", "Aric") { 0.7430186221096251,
                       auto ["1", "Dan") { 1.6208126320442937,
                       auto ["1", "Franck") { 0.42639707170035257,
                       auto ["Albert", "1") { 0.7430186221096251,
                       auto ["Albert", "Albert") { 2.4368257358712189,
                       auto ["Albert", "Aric") { 1.4368257358712191,
                       auto ["Albert", "Dan") { 2.0472097037446453,
                       auto ["Albert", "Franck") { 1.8340111678944691,
                       auto ["Aric", "1") { 0.7430186221096251,
                       auto ["Aric", "Albert") { 1.4368257358712191,
                       auto ["Aric", "Aric") { 2.4368257358712193,
                       auto ["Aric", "Dan") { 2.0472097037446457,
                       auto ["Aric", "Franck") { 1.8340111678944691,
                       auto ["Dan", "1") { 1.6208126320442937,
                       auto ["Dan", "Albert") { 2.0472097037446453,
                       auto ["Dan", "Aric") { 2.0472097037446457,
                       auto ["Dan", "Dan") { 3.1306328496328168,
                       auto ["Dan", "Franck") { 1.4860372442192515,
                       auto ["Franck", "1") { 0.42639707170035257,
                       auto ["Franck", "Albert") { 1.8340111678944691,
                       auto ["Franck", "Aric") { 1.8340111678944691,
                       auto ["Franck", "Dan") { 1.4860372442192515,
                       auto ["Franck", "Franck") { 2.3876142275231915}

        answer = defaultdict(dict);
        for (auto [k1, k2), v : answer_orig.items() {
            answer[k1][k2] = v

        G1 = xn::Graph([("Franck", "Aric"), ("Aric", "Dan"), ("Dan", "Albert"),
                       auto ["Albert", "Franck"), ("Dan", "1"), ("Franck", "Albert")]);

        result = communicability(G1);
        for (auto k1, val : result.items() {
            for (auto k2 : val) {
                assert_almost_equal(answer[k1][k2], result[k1][k2], places=7);

        result = communicability_exp(G1);
        for (auto k1, val : result.items() {
            for (auto k2 : val) {
                assert_almost_equal(answer[k1][k2], result[k1][k2], places=7);
