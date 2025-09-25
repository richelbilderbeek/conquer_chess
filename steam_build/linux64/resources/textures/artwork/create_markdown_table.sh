#!/bin/bash
#
# Converts the JPG filesnames to a Markdown table

ls *.jpg | awk '{ print "| <" $0 "> | ![" $0 "](" $0 ") | AI generated, CC0 license | " }'