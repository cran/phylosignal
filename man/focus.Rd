% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/barPhylo.R
\name{focusTraits}
\alias{focusTraits}
\alias{focusTree}
\alias{focusTips}
\alias{focusStop}
\title{Focus on sub parts of a plot}
\usage{
focusTraits(x)

focusTree()

focusTips()

focusStop()
}
\arguments{
\item{x}{the trait to focus on.
Can be a character string giving the name of the trait or an integer giving
the number of the trait in order of appearance in the plot.}
}
\description{
These functions can be used after \code{\link{barplot.phylo4d}}, \code{\link{dotplot.phylo4d}}
and \code{\link{gridplot.phylo4d}} when
\code{tree.type} is \code{"phylogram"} or \code{"cladogram"} to focus on
the different part of the plot and add graphical elements.
}
\details{
#' Use \code{focusTree} to focus on the phylogenetic tree, \code{focusTraits}
to focus on a given trait and \code{focusTips} to focus on the tips labels.
Use \code{focusStop} to close the editing and restore graphical settings.
For each part of the plot, the coordinate system is restored, making edition easier.
For the phylogeny, post-editing functions of the package \code{ape} like \code{nodelabels} can be used.
}
\examples{
require(ape)
require(phylobase)
data(navic)
dat <- tdata(navic)
neidium.sp <- c("Neidium bisulcatum",
                "Neidium affine",
                "Neidium productum")
stauroneis.sp <- c("Stauroneis kriegeri",
                   "Stauroneis acuta",
                   "Stauroneis gracilior",
                   "Stauroneis phoenicenteron")
neidium.mean <- mean(dat[neidium.sp,])
stauroneis.mean <- mean(dat[stauroneis.sp, ])

dotplot(navic, center = FALSE, scale = FALSE, data.xlim= c(0, 6))

focusTree()
nodelabels(node=c(22, 32), pch = 20, cex = 3, col = c(2, 3))

focusTraits()
segments(x0 = neidium.mean, y0 = 14.5,
         x1 = neidium.mean, y1 = 17.5,
         col = 3, lty = "dashed", lwd = 2)
segments(x0 = stauroneis.mean, y0 = 2.5,
         x1 = stauroneis.mean, y1 = 7.5,
         col = 2, lty = "dashed", lwd = 2)

focusTips()
rect(xleft = 0, ybottom = 2.5,
     xright = 0.9, ytop = 7.5,
     col = "#FF000020", border = NA)
rect(xleft = 0, ybottom = 14.5,
     xright = 0.9, ytop = 17.5,
     col = "#00FF0020", border = NA)

focusStop()

}
