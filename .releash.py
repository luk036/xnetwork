from releash import *

gitpush = ReleaseTargetGitPush('upstream', 'master')

xnetwork = add_package(path=".", name="xnetwork")
version_xnetwork = VersionSourceAndTargetHpp(xnetwork, '{path}/include/xnetwork/xnetwork_config.hpp', prefix='XNETWORK_VERSION_')
gittag_xnetwork = ReleaseTargetGitTagVersion(version_source=version_xnetwork, prefix='', annotate=True)

xnetwork.version_source = version_xnetwork
xnetwork.version_targets.append(version_xnetwork)

xnetwork.release_targets.append(gittag_xnetwork)
xnetwork.release_targets.append(gitpush)

source_tarball_filename = 'https://github.com/xtensor-stack/xnetwork/archive/{version}.tar.gz'.format(version=version_xnetwork)
xnetwork.release_targets.append(ReleaseTargetCondaForge(xnetwork, '../xnetwork-feedstock', source_tarball_filename=source_tarball_filename))

