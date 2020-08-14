
import os
import sys
import mock
import yaml
import re
from bs4 import BeautifulSoup as bs
import requests

sys.path.insert(0, os.path.abspath('../..'))
#import sphinx_rtd_theme

autodoc_mock_imports = ['pandas', 'numpy', 'pybind11', 
        'qufilab.indicators._trend', 'qufilab.indicators._volume',
        'qufilab.indicators._stat', 'qufilab.indicators._volatility',
        'qufilab.indicators._momentum', 'qufilab.patterns._bullish',
        'qufilab.patterns._bearish']

# -- Project information -----------------------------------------------------

project = 'qufilab'
copyright = '2020, Anton Normelius'
author = 'Anton Normelius'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ['sphinx.ext.autodoc', 'sphinx.ext.intersphinx', 'numpydoc', 
        'sphinx.ext.extlinks', 'autodocsumm']

# Uncomment line below if source should be included. 
#extensions.append('sphinx.ext.linkcode')
def linkcode_resolve(domain, info):
    if domain != 'py':
        return None

    if not info['module']:
        return None

    # Fullname contains the actual function that were called,
    # for example 'sma' in interface.py.
    fullname = info['fullname']
    
    # Parse indicators.yaml to find what script the implementation of 
    # fullname is in.
    with open("source/indicators.yaml", "r") as stream:
        try:
            data = yaml.safe_load(stream)
            for indicator_type, indicators in data.items():
                if fullname in indicators:
                    filename = "qufilab/indicators/_"+indicator_type+".cc"
                    break

        except yaml.YAMLError as exc:
            print(exc)

    # Open implementation file and check each line for start and end of implementation
    # Start is given by "Implementation of SMA" for example, and end is given by
    # "return sma", for example.
    github_url = "https://github.com/normelius/qufilab/blob/master/qufilab/" \
            "indicators/_{}.cc".format(indicator_type)
    
    r = requests.get(github_url)
    soup = bs(r.text, 'html.parser')
    text_parse = "Implementation of " + fullname.upper()
    try:
        line = soup.findAll('td', text = re.compile(text_parse))[0].get('id')
        lineno = re.sub('[C]', '', line)

    except:
        lineno = 1

    url = 'https://github.com/normelius/qufilab/blob/master/' \
            'qufilab/indicators/_{}.cc#{}'.format(indicator_type,
                    lineno)
    return url


# External links to be used for urls to correct source code part on github.
extlinks = {
        'trend': ('https://github.com/normelius/qufilab/blob/'\
        'master/qufilab/indicators/_trend.cc#%s', ''),
        'momentum': ('https://github.com/normelius/qufilab/blob/'\
        'master/qufilab/indicators/_momentum.cc#%s', ''),
        'volatility': ('https://github.com/normelius/qufilab/blob/'\
        'master/qufilab/indicators/_volatility.cc#%s', ''),
        'volume': ('https://github.com/normelius/qufilab/blob/'\
        'master/qufilab/indicators/_volume.cc#%s', ''),
        'stat': ('https://github.com/normelius/qufilab/blob/'\
        'master/qufilab/indicators/_stat.cc#%s', '')
        }


# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []

# Base index file
master_doc = 'index'

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'
#html_theme = 'default'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
html_css_files = [
    'custom.css',
]
