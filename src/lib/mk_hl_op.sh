#!/bin/bash

mkdir -p hl_ops
for x in $(cat ../include/extensions/math.h) ; do
    test "${x}" = "#ifdef" && continue
    test "${x}" = "#endif" && continue
    test "${x}" = "regFunc1" && continue
    xx=${x#regFunc1\(}
    fff=${xx%\)}
    file=hl_ops/${fff}.cc
    test -f ${file} && rm -f ${file}
    echo "#include \"multi_array_header.h\"" >> ${file}
    echo "#include \"hl_multi_array_header.h\"" >> ${file}
    echo "" >> ${file}
    echo "namespace MultiArrayTools" >> ${file}
    echo "{" >> ${file}
    echo "    template class HighLevelOp<OpCD,x_${fff}<double>,1>;" >> ${file}
    echo "    template class HighLevelOp<OpD,x_${fff}<double>,1>;" >> ${file}
    echo "    template HighLevelOpHolder<OpCD> hl_${fff} (const HighLevelOpHolder<OpCD>& in);" >> ${file}
    echo "    template HighLevelOpHolder<OpD> hl_${fff} (const HighLevelOpHolder<OpD>& in);" >> ${file}
    echo "}" >> ${file}
done

for fff in plus minus multiplies divides ; do
    file=hl_ops/${fff}.cc
    test -f ${file} && rm -f ${file}
    echo "#include \"multi_array_header.h\"" >> ${file}
    echo "#include \"hl_multi_array_header.h\"" >> ${file}
    echo "" >> ${file}
    echo "namespace MultiArrayTools" >> ${file}
    echo "{" >> ${file}
    echo "    template class HighLevelOp<OpCD,${fff}x<double,double>,2>;" >> ${file}
    echo "    template class HighLevelOp<OpD,${fff}x<double,double>,2>;" >> ${file}
    echo "}" >> ${file}
done

for fff in negate ; do
    file=hl_ops/${fff}.cc
    test -f ${file} && rm -f ${file}
    echo "#include \"multi_array_header.h\"" >> ${file}
    echo "#include \"hl_multi_array_header.h\"" >> ${file}
    echo "" >> ${file}
    echo "namespace MultiArrayTools" >> ${file}
    echo "{" >> ${file}
    echo "    template class HighLevelOp<OpCD,${fff}<double>,1>;" >> ${file}
    echo "    template class HighLevelOp<OpD,${fff}<double>,1>;" >> ${file}
    echo "}" >> ${file}
done
