from setuptools import find_packages, setup

package_name = 'Sensor_Package'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    package_data={'': ['py.typed']},
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='James Laliberte',
    maintainer_email='JAMES.LALIBERTE24@bathspa.ac.uk',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'sensor_bridge = Sensor_Package.Sensor_wifi_bridge:main',
            'decision_node = Sensor_Package.decision_node:main',
        ],
    },
)
