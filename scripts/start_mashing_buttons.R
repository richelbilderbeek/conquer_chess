#!/bin/env Rscript
if (1 == 2) {
  message("sudo apt install xdotool")
  remotes::install_github("pmur002/xdotool")
}

get_useful_keys <- function() {
  c(
    "a", "d", "s", "w", "q", "e", "z", "c",
    "i", "j", "k", "l", "u", "o", "m", "."
  )
}

get_random_key <- function() {
  sample(get_useful_keys(), 1)
}

run <- function(delay_msec = 20, n_presses = 1000000) {
  message("You have three seconds to put the Conquer Chess window in focus")
  Sys.sleep(3.0) # seconds

  i <- 0
  while (TRUE) {
    if (i == n_presses) return()
    key <- get_random_key()
    # message(i, ": pressing key: ", key)
    xdotool::keystroke(key, delay = delay_msec)
    i <- i + 1
  }
}

args <- commandArgs(trailingOnly = TRUE)
testthat::expect_true(length(args) == 0 || length(args) == 1)

delay_msec <- 20
if (length(args) == 1) {
  delay_msec <- args[1]
}
run(delay_msec = delay_msec)
