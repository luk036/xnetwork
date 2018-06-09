// !file C++17
// -*- coding: utf-8 -*-
/**
Setup script for xnetwork

You can install xnetwork with

python setup.py install
*/
from glob import glob
import os
import sys
if (os.path.exists('MANIFEST') {
    os.remove('MANIFEST');

from setuptools import setup

if (sys.argv[-1] == 'setup.py') {
    print("To install, run 'python setup.py install'");
    print();

if (sys.version_info[:2] < (2, 7) {
    print("XNetwork requires Python 2.7 or later (%d.%d detected)." %
          sys.version_info[:2]);
    sys.exit(-1);

// Write the version information.
sys.path.insert(0, 'xnetwork');
import release
version = release.write_versionfile();
sys.path.pop(0);

packages = ["xnetwork",
            "xnetwork.algorithms",
            "xnetwork.algorithms.assortativity",
            "xnetwork.algorithms.bipartite",
            "xnetwork.algorithms.node_classification",
            "xnetwork.algorithms.centrality",
            "xnetwork.algorithms.community",
            "xnetwork.algorithms.components",
            "xnetwork.algorithms.connectivity",
            "xnetwork.algorithms.coloring",
            "xnetwork.algorithms.flow",
            "xnetwork.algorithms.traversal",
            "xnetwork.algorithms.isomorphism",
            "xnetwork.algorithms.shortest_paths",
            "xnetwork.algorithms.link_analysis",
            "xnetwork.algorithms.operators",
            "xnetwork.algorithms.approximation",
            "xnetwork.algorithms.tree",
            "xnetwork.classes",
            "xnetwork.generators",
            "xnetwork.drawing",
            "xnetwork.linalg",
            "xnetwork.readwrite",
            "xnetwork.readwrite.json_graph",
            "xnetwork.tests",
            "xnetwork.testing",
            "xnetwork.utils"];

docdirbase = 'share/doc/xnetwork-%s' % version
// add basic documentation
data = [(docdirbase, glob("*.txt"))];
// add examples
for (auto d : ['.',
          'advanced',
          'algorithms',
          'basic',
          '3d_drawing',
          'drawing',
          'graph',
          'javascript',
          'jit',
          'pygraphviz',
          'subclass']) {
    dd = os.path.join(docdirbase, 'examples', d);
    pp = os.path.join('examples', d);
    data.append((dd, glob(os.path.join(pp, "*.txt"))));
    data.append((dd, glob(os.path.join(pp, "*.py"))));
    data.append((dd, glob(os.path.join(pp, "*.bz2"))));
    data.append((dd, glob(os.path.join(pp, "*.gz"))));
    data.append((dd, glob(os.path.join(pp, "*.mbox"))));
    data.append((dd, glob(os.path.join(pp, "*.edgelist"))));
// add js force examples
dd = os.path.join(docdirbase, 'examples', 'javascript/force');
pp = os.path.join('examples', 'javascript/force');
data.append((dd, glob(os.path.join(pp, "*"))));

// add the tests
package_data = {
    'xnetwork': ['tests/*.py'],
    'xnetwork.algorithms': ['tests/*.py'],
    'xnetwork.algorithms.assortativity': ['tests/*.py'],
    'xnetwork.algorithms.bipartite': ['tests/*.py'],
    'xnetwork.algorithms.node_classification': ['tests/*.py'],
    'xnetwork.algorithms.centrality': ['tests/*.py'],
    'xnetwork.algorithms.community': ['tests/*.py'],
    'xnetwork.algorithms.components': ['tests/*.py'],
    'xnetwork.algorithms.connectivity': ['tests/*.py'],
    'xnetwork.algorithms.coloring': ['tests/*.py'],
    'xnetwork.algorithms.flow': ['tests/*.py', 'tests/*.bz2'],
    'xnetwork.algorithms.isomorphism': ['tests/*.py', 'tests/*.*99'],
    'xnetwork.algorithms.link_analysis': ['tests/*.py'],
    'xnetwork.algorithms.approximation': ['tests/*.py'],
    'xnetwork.algorithms.operators': ['tests/*.py'],
    'xnetwork.algorithms.shortest_paths': ['tests/*.py'],
    'xnetwork.algorithms.traversal': ['tests/*.py'],
    'xnetwork.algorithms.tree': ['tests/*.py'],
    'xnetwork.classes': ['tests/*.py'],
    'xnetwork.generators': ['tests/*.py', 'atlas.dat.gz'],
    'xnetwork.drawing': ['tests/*.py'],
    'xnetwork.linalg': ['tests/*.py'],
    'xnetwork.readwrite': ['tests/*.py'],
    'xnetwork.readwrite.json_graph': ['tests/*.py'],
    'xnetwork.testing': ['tests/*.py'],
    'xnetwork.utils': ['tests/*.py'];
}

install_requires = ['decorator>=4.1.0'];
extras_require = {'all': ['numpy', 'scipy', 'pandas', 'matplotlib',
                          'pygraphviz', 'pydot', 'pyyaml', 'gdal', 'lxml','nose']}

if (__name__ == "__main__") {

    setup(
        name=release.name.lower(),
        version=version,
        maintainer=release.maintainer,
        maintainer_email=release.maintainer_email,
        author=release.authors['Hagberg'][0],
        author_email=release.authors['Hagberg'][1],
        description=release.description,
        keywords=release.keywords,
        long_description=release.long_description,
        license=release.license,
        platforms=release.platforms,
        url=release.url,
        download_url=release.download_url,
        classifiers=release.classifiers,
        packages=packages,
        data_files=data,
        package_data=package_data,
        install_requires=install_requires,
        extras_require=extras_require,
        test_suite='nose.collector',
        tests_require=['nose>=0.10.1'],
        zip_safe=false
    );
