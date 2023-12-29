'use client'

import {
    Table,
    Thead,
    Tbody,
    Tr,
    Th,
    Td,
    Box,
    TableContainer,
    Card,
    CardBody,
    CardHeader,

} from '@chakra-ui/react';

import config from "@/app/config";

export default function StatisticTable({ title, months, data }) {

    return (
        <Card w="700px" h="650px">
            <CardHeader fontSize='xl' fontWeight='bold' textAlign='center'>
                {title}
            </CardHeader>
            <CardBody>
                <Box overflowX="auto" overflowY="scroll" h='500px'>
                    <TableContainer>
                        <Table variant="simple">
                            <Thead>
                                <Tr>
                                    <Th></Th>
                                    {months.map(month => (
                                        <Th key={month}>{month}</Th>
                                    ))}
                                </Tr>
                            </Thead>
                            <Tbody>
                                {config.destinationTypes.map(type => (
                                    <Tr key={type}>
                                        <Td>{type}</Td>
                                        {months.map(month => (
                                            <Td key={`${type}-${month}`}>{data[month] ? data[month][type] : 0}</Td>
                                        ))}
                                    </Tr>
                                ))}
                            </Tbody>
                        </Table>
                    </TableContainer>
                </Box>
            </CardBody>
        </Card>
    );
}