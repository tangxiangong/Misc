using Documenter
 
makedocs(sitename = "DiffusionXX.jl",
    modules = [DiffusionXX],
    format = Documenter.HTML(
        prettyurls = false,
        mathengine = Documenter.HTMLWriter.MathJax3(Dict(
            :loader => Dict("load" => ["[tex]/physics"]),
            :tex => Dict(
            :equationNumbers => Dict(:autoNumber => "AMS"),
            "inlineMath" => [["\$","\$"], ["\\(","\\)"]],
            "tags" => "ams",
            "packages" => ["base", "ams", "autoload", "physics"],
        ),
    ))
    ),
    pages = [
        "Home" => "index.md",
        "Introduction" => "intro.md",
        "Manual" => Any[
            "man/guide.md"
        ],
        "references.md"
    ],    
)
